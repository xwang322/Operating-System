#include "cs537.h"
#include "pthread.h"
#include "server.h"

// CS537: Parse the new arguments too
void getargs(int *port, int argc, char *argv[])
{
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <port>\n", argv[0]);
    exit(1);
  }
  *port = atoi(argv[1]);
}

// For P4a - parse new arguments
void getnewargs(int *port, int *pool, int *buffer, int argc, char *argv[])
{
  if (argc != 4) {
    fprintf(stderr, "Usage: %s <port> <threads> <buffers>\n", argv[0]);
    exit(1);
  }
  *port = atoi(argv[1]);
  *pool = atoi(argv[2]);
  *buffer = atoi(argv[3]);
  if (*pool <= 0 || *buffer <= 0) {
    fprintf(stderr, "Invalid size! Usage: %s <port> <threads> <buffers>\n", argv[0]);
    exit(1);
  }
}

// This function handles incoming request and puts it into buffer
void fill_buffer(struct buffer *b, struct requestStat* newRequest) {
  b->requests[b->tail] = newRequest;
  b->curSize ++;
  b->tail ++;
  if (b->tail == b->maxSize) b->tail = 0;
}

// This function gets request from buffer
void get_buffer(struct buffer *b, struct requestStat **request) {
  *request = b->requests[b->head];
  b->curSize --;
  b->head ++;
  if (b->head == b->maxSize) b->head = 0;
}


// This producer function:
// Accept new http connections over the network 
// and place the descriptor for this connection into a fixed-size buffer
void producer(struct buffer *b, struct requestStat *newRequest) {
  pthread_mutex_lock(&m);
  while (b->curSize == b->maxSize) {
    pthread_cond_wait(&empty, &m);
  }
  fill_buffer(b, newRequest); // handles request and puts it into buffer
  pthread_cond_signal(&fill); 
  pthread_mutex_unlock(&m);
}

// This consumer function:
// A worker thread wakes when there is an http request in the queue. 
// Once the worker thread wakes, it performs the read on the network 
// descriptor, obtains the specified content (by either reading the static
// file or executing the CGI process), and then returns the content 
// to the client by writing to the descriptor. 
// The worker thread then waits for another http request.
void* consumer(void* arg) {
  struct buffer* b = (struct buffer*)arg;
  struct requestStat *request;
  
  while (1) {
    pthread_mutex_lock(&m);
    while (b->curSize == 0) {
      pthread_cond_wait(&fill, &m);
    }
    get_buffer(b, &request); // gets request from buffer
    pthread_cond_signal(&empty); 
    pthread_mutex_unlock(&m);
    
    requestHandle(request);
    Close(request->file_descriptor);
    
  }
  return NULL;
}

int main(int argc, char *argv[])
{
  int listenfd, connfd, port, clientlen;
  struct sockaddr_in clientaddr;
  
  //P4a new variables
  int poolMax = -1;
  int bufferMax = -1;
  
  getnewargs(&port, &poolMax, &bufferMax, argc, argv);
  
  // init a buffer struct
  struct buffer *b = malloc(sizeof(struct buffer*));
  b->requests = (struct requestStat**)malloc((sizeof(struct requestStat*))*bufferMax);
  b->maxSize = bufferMax;
  b->head = 0;
  b->tail = 0;
  b->curSize = 0;
  
  // create consumer threads
  pthread_t* consumers = (pthread_t*)malloc(sizeof(pthread_t) * poolMax);
  
  int i = 0;
  for (i=0; i<poolMax; i++) {
    int ret = pthread_create(&consumers[i], NULL, consumer, (void*)b);
    if (ret != 0){
      fprintf(stderr, "Thread is not created\n");
      exit(1);
    }
  }
  
  listenfd = Open_listenfd(port);
  while (1) {
    clientlen = sizeof(clientaddr);
    connfd = Accept(listenfd, (SA *)&clientaddr, (socklen_t *) &clientlen);
    
    struct requestStat *newRequest = malloc(sizeof(struct requestStat));
    newRequest->file_descriptor = connfd;
    producer(b, newRequest);
 
  }
  
}





 
