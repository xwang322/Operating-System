#include "request.h"
#include "pthread.h"

#ifndef __SERVER_H__

struct buffer{
  int head;
  int tail;
  int curSize; //current size of buffer
  int maxSize; //max size of buffer
  struct requestStat **requests; //queue
};

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t fill = PTHREAD_COND_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;

#endif
