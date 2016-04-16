#include "cs537.h"
#include "server.h"
#include <pthread.h>


void setQueue(volatile struct queue* q, int buffers)
{
  q->current_size = 0;
  q->max_size = buffers;
  q->head = 0;
  q->tail = 0;
  q->request = (struct requestStat*) malloc(sizeof(struct requestStat) * buffers);
}

void getargs(int *port, int argc, char *argv[])
{   
  if (argc != 4) {
    fprintf(stderr, "Usage: %s <port> <threads> <buffers>\n", argv[0]);
    exit(1);
  }
  int tmp1 = atoi(argv[2]);
  int tmp2 = atoi(argv[3]);
  if (tmp1 <= 0 || tmp2 <= 0){
    fprintf(stderr, "Usage: %s <port> <threads> <buffers>\n", argv[0]);
    exit(1);
  }
}

void* producer(int connfd)
{ 
  pthread_mutex_lock(&m);
  while (q.current_size == q.max_size){
    pthread_cond_wait(&empty, &m);
  } 
  struct requestStat r;
  r.file_descriptor = connfd;
  fill(&q, r);
  pthread_cond_signal(&full);  
  pthread_mutex_unlock(&m);
  return NULL;
}

void* consumer(void *arg)
{
  while(1){
    pthread_mutex_lock(&m);
    while (q.current_size == 0){
      pthread_cond_wait(&full, &m);
    } 
    struct requestStat r = get(&q);
    pthread_cond_signal(&empty);
    pthread_mutex_unlock(&m);
    requestHandleConsumer(&r);
    Close(r.file_descriptor);
  }
  return NULL;
}

void fill(volatile struct queue* q, struct requestStat r)
{   
  q->request[q->tail] = r;
  q->current_size++;
  q->tail++;
  if (q->tail == q->max_size){
    q->tail = 0;
  }
}

struct requestStat get(volatile struct queue* q)
{
  struct requestStat r = q->request[q->head];
  q->current_size--;
  q->head++;
  if (q->head == q->max_size){
    q->head = 0;
  }
  return r;
}

void requestHandleConsumer(struct requestStat* r)
{
  int fd = r->file_descriptor;
  int is_static;
  struct stat sbuf;
  char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
  char filename[MAXLINE], cgiargs[MAXLINE];
  rio_t rio;
  
  Rio_readinitb(&rio, fd);
  Rio_readlineb(&rio, buf, MAXLINE);
  sscanf(buf, "%s %s %s", method, uri, version);
  
  printf("%s %s %s\n", method, uri, version);
  
  if (strcasecmp(method, "GET")) {
    requestError(fd, method, "501", "Not Implemented", "CS537 Server does not implement this method");
    return;
  }
  requestReadhdrs(&rio);
  
  is_static = requestParseURI(uri, filename, cgiargs);
  if (stat(filename, &sbuf) < 0) {
    requestError(fd, filename, "404", "Not found", "CS537 Server could not find this file");
    return;
  }
  
  if (is_static) {
    if (!(S_ISREG(sbuf.st_mode)) || !(S_IRUSR & sbuf.st_mode)) {
      requestError(fd, filename, "403", "Forbidden", "CS537 Server could not read this file");
      return;
    }
    requestServeStatic(fd, filename, sbuf.st_size);
  } else {
    if (!(S_ISREG(sbuf.st_mode)) || !(S_IXUSR & sbuf.st_mode)) {
      requestError(fd, filename, "403", "Forbidden", "CS537 Server could not run this CGI program");
      return;
    }
    requestServeDynamic(fd, filename, cgiargs);
  }
}

int main(int argc, char *argv[])
{
  int listenfd, connfd, port, clientlen;
  struct sockaddr_in clientaddr;
  
  getargs(&port, argc, argv);
  int pools, buffers;
  
  port = atoi(argv[1]);
  pools = atoi(argv[2]);
  buffers = atoi(argv[3]);
  
  setQueue(&q, buffers);
  
  pthread_t* consumers = (pthread_t*)malloc(sizeof(pthread_t) * pools);
  
  int i;
  for (i = 0; i < pools; i++){
    int ret = pthread_create(&consumers[i], NULL, consumer, NULL); 
    if (ret != 0){
      fprintf(stderr, "Thread is not created\n");
      exit(1);
    }
  }
  
  listenfd = Open_listenfd(port);
  while (1) {
    clientlen = sizeof(clientaddr);
    connfd = Accept(listenfd, (SA *)&clientaddr, (socklen_t *) &clientlen);
    producer(connfd);  	
  }
  return 0;
}
