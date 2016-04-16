#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
/*
int
recursion(int init, int cycles){

  int i = 0; 
  int array[cycles];
  array[0] = 0;
  // printf(1, "i is %d\n", i);
  for(i = 1; i < cycles; i++){
    array[i] = array[i-1] + i;
  }

  // printf(1, "last array is %d\n", array[cycles-2]);
  return array[cycles-2];
}


int
main(int argc, char *argv[]){

  int a = 0;
  // printf(1, "initial value is: %d\n", a);
  int b = 500;
  // printf(1, "cycling times is: %d\n", b);
  int sum = recursion(a,b);
  printf(1, "last array element value: %d\n", sum);
  printf(1, "last array element address: %x\n", (int)&sum);
 
  if((int)&sum < 640*1024 && (int)&sum > 636*1024){
    printf(1, "address unreasonable, test failed\n");
  } 
  if((int)&sum < 636*1024){
    printf(1, "address reasonable, test passed\n");
  } 

  exit();
}
*/
int sum(int n){
    if(n==0)
       return n;
    else
       return n+sum(n-1);    
}

int main(int argc,char *argv[])
{
    int n=1000;
    printf(1,"sum of %d is %d\n",n,sum(n));
    exit();
}
