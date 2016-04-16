#include "types.h"
#include "user.h"

#define LENGTH 2048 

void
overgrowstack(int loop)
{
  if(loop == 0)
    return;
  // printf(1, "iteration left: %d\n", loop);
  int matrix[LENGTH];
  matrix[0] = 10;
  overgrowstack(loop - 1);
}

int
main(int argc, char *argv[])
{
  overgrowstack(400);
  exit();
}
