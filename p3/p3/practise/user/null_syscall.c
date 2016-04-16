#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int
main(int argc, char *argv[])
{
  int *p = 0;
  int rc1 = write(1, (void *)p, 1);
  int rc2 = read(1, (void *)p, 1);
  if(rc1 == -1 && rc2 == -1){
  printf(1, "TEST PASSED\n");
  }
  exit();
}
