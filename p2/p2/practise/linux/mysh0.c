#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>


#define in_size 512


// main function

   // able to record the last 20 commands
   // redirection: justify and tell the where it goes
   // white spaces
   // batch mode
   // exit
   // history

//define the global variables
char prompt_message[7] = "mysh # ";
char error_message[30] = "An error has occured\n";
char *input[512];
char *void_input[512];
char *tmp_input[512];
char *str_exit = "exit";
char *str_pwd = "pwd";
char *str_cd = "cd";
char batch_sep[512];
char current_path[512];
char *pre_redir = NULL;
char *post_redir = NULL;
int is_batch = 0;
int is_internal = 0;
int is_redir = 0;
int in_size = 512;
int word_cnt = 0;
//some possible needed variables
int count = 0;

// print the pormpt command
void
print_prompt(){
   if (!is_batch){
      write(STDOUT_FILENO, prompt_message, strlen(prompt_message));         
   }
}

// Error handling: the one and only error message
void
print_error(){
    write(STDERR_FILENO, error_message, strlen(error_message));
}

// need the function to split and line and do the prepocessing work
int
sep_line(char *line, char *separated[]){
    char *input = strdup(line);
    int count;
    while (1){
        while (isspace(*input){
            input++;
        }
        if (*(input) == '\0'){
           seprated[count++] = input;   
        }
    }


}

// compare two string
int
compare(char *str1, char *str2){
    while (*str1 && *str2){ // means that they can get a value from * function
        if (*str1 != *str2){
            return 0;
        }
        str1++; str2++;
    }
    return 1;
}

int
main (int argc, char *argv[]){
    print_prompt();  
    FILE *stream = NULL;   // how to define a FILE type?
    // may need more variable, we will see

    // Judge a file is a batch or not
    if (argc == 1){
       // deal with stream or input;
    }
    else if (argc == 2){
        is_batch = 1;
        input = fopen(argv[1], "r");
        if (input == NULL){
            print_error();
            exit(1);
        }
     else{
         print_error();
         exit(1);
     }
    }

   // Tell whether it is a valid input command
    while (fgets(input, in_size, stream)){
        // if the input is too large
        if (strlen(input) > 513){ // 513 includes the ending, total is 513?
            if (is_batch){ // how to deal with batch situation close stanard output and replace with batch
                //write(STDOUT_FILENO, input, sizeof(input));
                print_error();
                print_prompt();
            }
            print_error();
            print_prompt();    
        }
       // if the input is empty
       if (compare(input, void_input) == 1){ //strcmp cannot compare a null string with another null string
           print_prompt();   
       }
    }
    // redirection
    tmp_input = strdup(input);
    pre_redir = strtok(tmp_input, ">");
    if (strlen(pre_redir) != strlen(input)){
        is_redir =1; 
        post_redir = strtok(NULL, ">");
        if (strtok(NULL, ">")){ // too many input
            print_error();
            print_prompt();
        }
    }
    

    // exit
    if (!strcmp(str_exit, input)){
        if (word_cnt != 1){
            print_error();
            print_prompt();
        }
        else {
            exit(0);
        }        
    }
    exit(0);

    // fork and execvp
    if (is_internal != 0){
        child = fork();
        if (child == 0){
            execvp(); // needs writing
        }
        if (child != 0){
            print_error();
        }
    }
        
}







