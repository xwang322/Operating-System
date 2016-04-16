#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

int
main(int argc, char *argv[]){

  int a = 1000;
  int b = 2000;
  char message[20];
  // char *mess_addr;
  // mess_addr = (char *)malloc(sizeof(message)*20);
  // strcpy(message, "this is a test");
  // printf(1, "%d\n", &a);
  // printf(1, "%d\n", &b);
  // printf(1, "%s\n", mess_addr);
  // printf(1, "Address %d, evaluated expression: %s\n", &message, message);
  // printf(1, "%x\n", (int)&a);
  // printf(1, "%x\n", (int)&b);

  if((int)&a < 640*1024 && (int)&a > 636*1024){
    if((int)&b < 640*1024 && (int)&b > 636*1024){
      if((int)&message < 640*1024 && (int)message > 636*1024){
        printf(1, "addresses reasonable, test passed\n"); 
      }
    } 
  }
  exit();
}
