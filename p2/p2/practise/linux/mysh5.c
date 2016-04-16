#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>


#define in_size 512
#define History_Max 20  //new history

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
char *words[512];
//char batch_sep[512];
//char current_path[512];
char *pre_redir = NULL;
char *post_redir = NULL;
int is_batch = 0;
int is_internal = 0;
int is_redir = 0;
int word_cnt = 0;
unsigned int his_num = 1;  //new added history number
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
    int count = 0;
    while (1){
        while (isspace(*input)){ // getting rid of all possible blanks before a real useful word comes
            input++;
        }
        if (*input == '\0'){
            return count;   
        }
        separated[count++] = input;
        while (!isspace(*input) && (*input != '\0')){
            input++;
        }
        if (*input != '\0'){
            *input = '\0';
            input++; 
            return count; 
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
       	    index++
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
        // if the input is empty
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
        if (!is_redit){ // if this is not redirection
            word_cnt = sep_line(input, words);
        }
        else{ // if this is a redirection
            word_cnt = sep_line(pre_redir, words);
            j = sep_line(post_redir, words);
            word_cnt += j; 
            if (j < 1){ // means there is no output file defined
                print_error();
                print_prompt();
                continue;
            }
            if (j > 2){ // means there are too many files defined
                print_error();
                print_prompt();
                continue;
            }
            // problems with sep_line coukd count either 1 or 2 strings. will tackle later    
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
            fd_out = open(, O_WRONLY|O_CREAT|O-TRUNC, S_IRWXU); // need another input, will see
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

        // fork and execvp, how to deal with fork and child is still undefined
        if (is_internal != 0){
            int  child = fork();
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







