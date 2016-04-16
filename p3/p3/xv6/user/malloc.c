#include "types.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  int *a = (int *)malloc(sizeof(int));
  *a = 100;
  // printf(1, "%d\n", *a);
  // printf(1, "%d\n", a);
  int *b = (int *)malloc(sizeof(int));
  *b = 150;
  // printf(1, "%d\n", *b);
  // printf(1, "%d\n", b);
  char *c = (char *)malloc(sizeof(char));
  c = "this is a test\n";
  // printf(1, "%d\n", *c);
  // printf(1, "%d\n", c);
  if(a && *a && b && *b && c && *c){
    printf(1, "TEST PASSED\n");
  }
  exit();
}
