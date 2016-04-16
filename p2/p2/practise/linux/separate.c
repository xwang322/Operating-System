#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>


#define MAX_SIZE 1000
int split_up(char* string, char* argv[])
{
    char* p = string;
    int argc = 0;

    while(*p != '\0')
    {
        while(isspace(*p))
            ++p;
        if(*p != '\0'){
          
 		 argv[argc++] = p;
		printf("%d\n",argc-1); 
          		printf("%s\n",argv[argc-1]); 
	}
        else
            break;

        while(*p != '\0' && !isspace(*p))
            p++;
        if(*p != '\0')
        {
          *p = '\0';
          ++p;
         }
   }

    return argc;
}

int main()
{
    char *av[MAX_SIZE];
    char string[] = "this is a test";
    int i;
    int ac = split_up(string, av);
    printf("The number of token is: %d\n", ac);
	printf("\"%s%s\"\n", av);
    for(i = 0; i < ac; i++)
        printf("\"%d\"\n", av[i][1]);
    return 0;
}
