#include "types.h"
#include "stat.h"
#include "user.h"
#include "pstat.h"
#define check(exp, msg) if(exp) {} else {\
   printf(1, "%s:%d check (" #exp ") failed: %s\n", __FILE__, __LINE__, msg);\
   exit();}

int
main(int argc, char *argv[])
{
   struct pstat st;

   //sleep(20); //change here based on requirement
   check(getpinfo(&st) == 0, "getpinfo");
   
   int count = 0;
   int i, j;
   for(i = 0; i < NPROC; i++) {
      if (st.inuse[i]) {
	  count++;
	  printf(1, "pid: %d priority: %d\n ", st.pid[i], st.priority[i]);
	  for (j = 0; j < 4; j++)
	      printf(1, "\t level %d ticks used %d\n", j, st.ticks[i][j]);  
         check(st.ticks[i][0] > 0, "all processes must have run at least once");
	 check(st.priority[i] == 3, "all processes must have lowest priority");
      }
   }

   check(count == 3, "should be three processes: init, sh, tester");

   printf(1, "TEST PASSED");
   exit();
}
