#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

int
main(int argc, char *argv[]){

  int a = 1000;
  int b = 2000;
  char message[20];
 
  if((int)&a < 640*1024 && (int)&a > 636*1024){
    if((int)&b < 640*1024 && (int)&b > 636*1024){
      if((int)&message < 640*1024 && (int)message > 636*1024){
        printf(1, "TEST PASSED\n"); 
      }
    } 
  }
  exit();
}
