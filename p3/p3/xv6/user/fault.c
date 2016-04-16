#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

int
main(int argc, char *argv[]){
 
  char *p = (char *)((641*1024));  // access address space above USERTOP
  printf(1, "%x\n", *p);
  exit();
}
