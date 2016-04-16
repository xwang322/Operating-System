#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  char *p = (char *)((320*1024));
  int rc = write(1, (void *)p, 1);
  if(rc == -1){
  printf(1, "TEST PASSED\n");
  }
  exit();
}
