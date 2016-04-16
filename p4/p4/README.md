# CS537_P4

Name: Xiaodong Wang

CS Login: xiaodong

UW ID: 9066383432

email: xwang322@wisc.edu

Name: Zhen Di

CS Login: zdi

UW ID:  di3

email: di3@wisc.edu

For p4a, we have created a new file called server.h, which serves as function of initialized thread lock and condition variables, we defined a new data

structure called buffer which is very similar to queue serving as buffer based on requirement. We also created a file called request.h, which redefine 

the requset from client, we will save not only file descriptor but also file name and error code. The last thing we did is updating server.c, we 

followed the producer and consumer codes in slides but made changed in argument list to achieve receiving and handling requests. Our codes passed all 

the tests.


For p4b, we made a bunch of changes:

./kernel/proc.c: add syscall "clone" and "join", revise "wait" "growproc" and "exit";

./include/syscall.h: update the new added syscalls;

./kernel/syscall.c: add new syscalls;

./kernel/sysproc.c: judge the new syscall will find the right pointers;

./def.h: add new variables in def header;

./user/ulib.c: add thread_create and thread_join for thread, lock_init, lock_acquire and lock_release for the requirement;

We built fine and passed all tests.

