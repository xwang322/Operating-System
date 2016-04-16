#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

int sum(int n){
  if(n == 0)
    return n;
  else
    return n+sum(n-1);    
}

int main(int argc,char *argv[])
{
  int n = 2000;
  int correct = n*(n+1) / 2;
  if(correct == sum(n)){
    printf(1,"TEST PASSED\n");
  }
  exit();
}
