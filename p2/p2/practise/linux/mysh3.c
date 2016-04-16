#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>

#define in_size 512
#define History_Max 20  

// main function

   // able to record the last 20 commands
   // batch mode
   // history

//define the global variables
char prompt_message[7] = "mysh # ";
char error_message[30] = "An error has occured\n";
char *input[512];
char *void_input[512] ;
char *tmp_input = NULL;
char *str_exit = "exit";
char *words[512];
//char batch_sep[512];
//char current_path[512];
char *pre_redir = NULL;
char *post_redir = NULL;
int is_batch = 0;
int is_internal = 0;
int is_redir = 0;
int word_cnt = 0;
unsigned int his_num = 1;  

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

// need the function to split the line and count how many words in this string
int
sep_line(char *input, char *separated[]){
    char *line = strdup(input);
    int count = 0;
    while (1){
        while (isspace(*line)){ // getting rid of all possible blanks before a real useful word comes
            line++;
        }
        if (*line == '\0'){
            return count;   
        }
        separated[count++] = line;
        while (!isspace(*line) && (*line != '\0')){
            line++;
        }
        if (*line != '\0'){  // when the next one is a space, we will just put an EOF to end it
            *line = '\0';
            line++; 
            return count; 
        }
}

// compare two string
int
compare(char *str1, char *str2){
    while (*str1 && *str2){ // means that they can get a value from * function, strcmp cannot compare NULL
        if (*str1 != *str2){
            return 0;
        }
        str1++; str2++;
    }
    return 1;
}

// history 
void
print_history(char *history[], int current){
    int i = current;
    int index;
    if (his_num <= 20){
        index = 1;
    }
    else{
        index = his_num - 19;
    }
    do{
        if (history[i]){
       	    printf("%d %s\n", index, history[i]);
       	    index++;
	}
	i = i + 1 % History_Max;
    }while (i != current);
}

int
clear_history(char *history[]){
    int i;
    for (i = 0; i < History_Max; i++){
        free(history[i]);
	history[i] = NULL;
    }
    return 0;
}

// main function
int
main (int argc, char *argv[]){
    print_prompt();  
    FILE *stream = NULL;
    char *history[History_Max];
    int i;
    int current = 0;
    for(i = 0; i < History_Max; i++){
        history[i] = NULL;
    }   
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
                continue;
            }
            print_error();
            print_prompt();
            continue;    
        }
        // if the input is empty, output the shell command
        if (compare(input, void_input) == 1){ //strcmp cannot compare a null string with another null string
            print_prompt();
            continue;   
        }

        // redirection
        tmp_input = strdup(input);
        pre_redir = strtok(tmp_input, ">");
        if (strlen(pre_redir) != strlen(input)){
            is_redir =1; 
            post_redir = strtok(NULL, ">"); // why use NULL?
            if (strtok(NULL, ">")){ // too many input
                print_error();
                print_prompt();
                continue;
            }
        }
 
        int j = 0; // j means how many words after the token
        // deal with redirection or not
        if (!is_redir){ // if this is not redirection
            word_cnt = sep_line(input, words);
        }
        else{ // if this is a redirection
            word_cnt = sep_line(pre_redir, words);
            j = sep_line(post_redir, words);
            word_cnt += j; 
            if (j != 1){ // means there is either no or more than output file defined, which is wrong
                print_error();
                print_prompt();
                continue;
            }  
        }
        
	//create history
	if(input[strlen(input) - 1] == '\n') 
		input[strlen(input) - 1] = '\0';
	free(history[current]);
	history[current]  = strdup(input);
	current  = (current + 1) % History_Max;

        // check for redirection output file is valid or not
        int fd_out = 0;
        if (is_redir){
            fd_out = open(words[word_cnt-1], O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU); // need another input, will see
            if (fd_out < 0){
                print_error();
                print_prompt();
                continue; 
            }
            else if (){ // deal with file_descriptor case, will see, another possible error
                print_error(); 
                print_prompt();
                continue;
            }
        }

        // Internal command
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
       
        int child; 
        // fork and execvp, how to deal with fork and child is still undefined
        if (is_internal != 0){
            child = fork();
            if (child == 0){
                execvp(); // needs writing
            }
            if (child != 0){
                print_error();
            }
        }
        // probably need to deal with output
    print_prompt();
    }//end while loop

    return 0;        
}







