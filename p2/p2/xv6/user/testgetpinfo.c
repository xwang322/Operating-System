#include "types.h"
#include "stat.h"
#include "user.h"
#include "pstat.h"
#define check(exp, msg) if(exp) {} else {\
   printf(1, "%s:%d check (" #exp ") failed: %s\n", __FILE__, __LINE__, msg);\
   exit();}


int
spin(){
  int a, b, y = 0;
  while (y < 1000){
    y++;
    for (a = 0; a < 10000; a++) {
      b = a % 11;
    }
 }
 return b;
}


int
main(int argc, char *argv[]){  
  struct pstat st;
  int pids[2];
  int ppid = getpid();
  int r, i;

  pids[0] = fork();
  if (pids[0] == 0) {
    r = getpinfo(&st);
    if (r != 0) {
      kill(ppid);
    }
    spin();
  }

  pids[1] = fork();
  if (pids[1] == 0) {
    r = getpinfo(&st);
    if (r != 0) {
      kill(ppid);
    }
    spin();
  }

  sleep(50);
  check(getpinfo(&st) == 0, "getpinfo");
  
  for (i = 0; i <2;  i++){
    kill(pids[i]);
    wait();  
  } 

  exit();
}
