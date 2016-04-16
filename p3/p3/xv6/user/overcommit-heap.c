#include "types.h"
#include "user.h"

#define SIZE 1000

int main(int argc,char *argv[])
{
  int N = 1000;
  int *matrix[N];
  int n;
  for(n = 0;n < N; n++){
    matrix[n] = malloc(sizeof(int)*SIZE);
  }  
  for(n = 0;n < N; n++){
    free(matrix[n]);
  }
    exit();
}
