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
     int i;

    while(*p != '\0')
    {
        while(isspace(*p))
            ++p;

        if(*p != '\0'){
            argv[argc++] = p;
	    printf("\"%s\"\n", p);
//	for (i=argc;i>0;i--){
 //          printf("\"%s\"\n", argv[i-1]);
//	}
	}
        else
            break;

        while(*p != '\0' && !isspace(*p))
            p++;
        if(*p != '\0')
        {
          *p = '\0';
          ++p;
    //     printf("\"%s\"\n", argv[argc-1]);

        }
   }

    return argc;
}

int main()
{
    char *av[MAX_SIZE];
    char string[] = "this is a test";
    //int i, ac = makeargv(string, av, 2);
    int i;
    int ac = split_up(string, av);
    printf("The number of token is: %d\n", ac);
    for(i = 0; i < ac; i++)
        printf("\"%c\"\n", *av[i]);

    return 0;
}
