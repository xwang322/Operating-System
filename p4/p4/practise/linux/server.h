#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "request.h"
#include <pthread.h>

struct requestStat
{
     //int is_static; //whether request is static(1) or dynamic(0)
     //char filename[MAXLINE];
     int file_descriptor;
     //int error_code;
     // may need to add more variables here
};

struct queue
{
    int current_size;
    int max_size;
    int head;
    int tail;
    struct requestStat* request;
};

volatile struct queue q;

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t full = PTHREAD_COND_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;

void setQueue(volatile struct queue* q, int buffers);
void getargs(int* stats,int argc, char *argv[]);
void* producer(int connfd);
void* consumer(void *arg);
void fill(volatile struct queue* q, struct requestStat r);
struct requestStat get(volatile struct queue* q);
void requestHandleConsumer(struct requestStat* r);

