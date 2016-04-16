#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"


int
main(int argc, char *argv[])
{
  int n = getsyscallnumtotal();
  int m = getsyscallnumsuccess();
  printf(1,"%d %d\n", n, m);
  exit();
}
