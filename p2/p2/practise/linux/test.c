#include <string.h>
#include <stdio.h>

int main()
{
   char str[80] = "This is -www.tutorialspoint.com";
   const char s[2] = "-";
   char *token;
   
   /* get the first token */
   token = strtok(str, s);
 //  printf( " %s\n", token );
   /* walk through other tokens */
   while( token != NULL ) 
   {
      printf( " %s\n", token );
    
      token = strtok(NULL, s);
      printf( "test: %s\n", token );
   }
   
   return(0);
}
