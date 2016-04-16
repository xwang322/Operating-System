// Do not modify this file. It will be replaced by the grading scripts
// when checking your project.

#include "types.h"
#include "stat.h"
#include "user.h"

#define FAIL(x)\
   printf(1, "failed: %s\n", x);\
   exit();\

/* Created this macro to increase confidence in whether a test is passing
 * or failing. If they count too few system calls for a test, then it definitely
 * failed. If they count too many, it may be because another process got CPU
 * time and made additional system call/s.
 */
#define TEST(CODE, EXPECTED, TESTS, MSG)                                 \
  printf(1, "calling (%s), should count %d successful syscalls\n", MSG, EXPECTED);\
  failed = passed = 0;\
  for(i = 0; i < TESTS; i++) {\
    before = getsyscallnumsuccess();\
    CODE;\
    after = getsyscallnumsuccess() - 1;\
    printf(1, "%d) counted %d successful syscalls\n", i+1, after-before);\
    if(after - before < EXPECTED) { FAIL("count too low"); }\
    else if(after - before == EXPECTED) { passed++; }\
    else { failed++; }\
  }\
  if (failed > passed) { FAIL("count consistently too high"); };

int
main(int argc, char *argv[])
{
  int i, failed, passed;
  int before, after;

  // fork, exit, wait (kind of hard to test after a fork otherwise due to race conditions)
  //int pid;
  //TEST(pid = fork(); if(pid == 0) exit(); else wait();, 3, 3, "fork, exit, wait");

  // pipe
  int fd[4];
  TEST(pipe(fd);, 1, 3, "pipe");
  
  // write
  TEST(write(fd[1], "hello", 5);, 1, 3, "write");

  // read
  char str[5];
  TEST(read(-1, str, 1);, 0, 3, "read");

  // close
  TEST(close(fd[0]); close(fd[1]);, 2, 1, "close, close");
  TEST(close(12);, 0, 3, "close");

  // getpid
  //  TEST(pid = getpid();, 1, 3, "getpid");

  // sleep
  TEST(sleep(10);, 1, 3, "sleep");

  // uptime
  TEST(uptime();, 1, 3, "uptime");

  // open, dup
  TEST(fd[2] = open("invalid", 0); fd[3] = dup(fd[2]);, 0, 3, "open, dup");

  // fstat
  struct stat fs;
  TEST(fstat(fd[2], &fs);, 0, 3, "fstat");

  // mkdir
  TEST(mkdir("syscalltest");, 1, 1, "mkdir");

  // chdir
  TEST(chdir("syscalltest");, 1, 1, "chdir");
  TEST(chdir("dne");, 0, 3, "chdir");

  // link
  TEST(link("badfile", "../nonexistant.txt");, 0, 3, "link");

  // unlink
  TEST(unlink("junk.txt");, 0, 3, "unlink");

  // now remove the directory we created
  chdir("..");
  unlink("syscalltest");

  // mknod
  TEST(mknod("syscalltest", 1, 1);, 1, 1, "mknod");
  unlink("syscalltest");

  // sbrk
  TEST(sbrk(0);, 1, 1, "sbrk");

  printf(1, "TEST PASSED");
  exit();
}
