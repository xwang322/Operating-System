#include "types.h"
#include "stat.h"
#include "fcntl.h"
#include "user.h"
#include "x86.h"

#define PGSIZE 4096 //bytes mapped by a page

char*
strcpy(char *s, char *t)
{
  char *os;

  os = s;
  while((*s++ = *t++) != 0)
    ;
  return os;
}

int
strcmp(const char *p, const char *q)
{
  while(*p && *p == *q)
    p++, q++;
  return (uchar)*p - (uchar)*q;
}

uint
strlen(char *s)
{
  int n;

  for(n = 0; s[n]; n++)
    ;
  return n;
}

void*
memset(void *dst, int c, uint n)
{
  stosb(dst, c, n);
  return dst;
}

char*
strchr(const char *s, char c)
{
  for(; *s; s++)
    if(*s == c)
      return (char*)s;
  return 0;
}

char*
gets(char *buf, int max)
{
  int i, cc;
  char c;

  for(i=0; i+1 < max; ){
    cc = read(0, &c, 1);
    if(cc < 1)
      break;
    buf[i++] = c;
    if(c == '\n' || c == '\r')
      break;
  }
  buf[i] = '\0';
  return buf;
}

int
stat(char *n, struct stat *st)
{
  int fd;
  int r;

  fd = open(n, O_RDONLY);
  if(fd < 0)
    return -1;
  r = fstat(fd, st);
  close(fd);
  return r;
}

int
atoi(const char *s)
{
  int n;

  n = 0;
  while('0' <= *s && *s <= '9')
    n = n*10 + *s++ - '0';
  return n;
}

void*
memmove(void *vdst, void *vsrc, int n)
{
  char *dst, *src;
  
  dst = vdst;
  src = vsrc;
  while(n-- > 0)
    *dst++ = *src++;
  return vdst;
}

// P4b: creates a user thread by calling syscall clone()
int thread_create(void(*fcn)(void*), void *arg) {
  void *stack = malloc(2*PGSIZE);
  // page alignment
  stack = stack + (PGSIZE - (uint)stack%PGSIZE);
  return (clone(fcn, arg, stack));
}

// P4b: calls the underlying join() system call, frees the user stack, 
// and then returns.
int thread_join() {
  //void *stack;
  //int ret = join((&stack));
  //free(stack);
  //return ret;
  void* stack;
  int thd_join;
  thd_join = join(&(stack));
  free(stack);  // free has to pass in void* type, so we define stack as void* as same as with clone;
  return thd_join;
}

// initialize lock, acquire lock and release lock, xchg is defined in x86.h already

void 
lock_init(lock_t *lock)
{
  lock->flag = 0;
}

void 
lock_acquire(lock_t *lock)
{
  while(xchg((volatile uint*)&lock->flag, 1) == 1)
    ;
}

void 
lock_release(lock_t *lock)
{
  lock->flag = 0;
}
