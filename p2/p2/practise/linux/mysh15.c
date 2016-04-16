#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>

#define in_size 512
#define History_Max 20  

// Define the global variables
char prompt_message[7] = "mysh # ";
char error_message[30] = "An error has occured\n";
char input[512];
char *words[512];
int is_batch = 0;
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

// Need the function to split the line and count how many words in this string
int
sep_line(char *input, char *words[]){
    char *line = strdup(input);
    int count = 0;
    while (1){
        while (isspace(*line)){ // getting rid of all possible blanks before a real useful word comes
            line++;
        }
        if (*line != '\0'){
	words[count++] = line;
        }
	else {
	return count;
	}
        while (!isspace(*line) && (*line != '\0')){
            line++;
        }
        if (*line != '\0'){  // when the next one is a space, we will just put an EOF to end it
            *line = '\0';
	    line++;
        }
    }
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
	i = (i + 1) % History_Max;
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

int
clear_words(char *words[]){
    int i;
    for (i = 0; i < word_cnt; i++){
	words[i] = NULL;
    }
    return 0;
}


// main function
int 
main (int argc, char *argv[]){

    FILE *stream = NULL;
    //char void_input[512];
    char *tmp_input = NULL;
    char *history[History_Max]; 
    int is_internal = 0;
    int is_redir = 0;
    int i;
    int current = 0;
    for(i = 0; i < History_Max; i++){
        history[i] = NULL;    
    }   
    // Judge a file is a batch or not
    if (argc == 1){
        stream = stdin; 
        print_prompt();  
    }
    else if (argc == 2){
        is_batch = 1;
        write(STDOUT_FILENO, input, strlen(input));
        stream = fopen(argv[1], "r");
        if (stream == NULL){
            print_error();
            exit(1);
        }
        print_prompt();  
    }
    else {
         print_error();
         exit(1);
    }

   // Tell whether it is a valid input command
    while (fgets(input, in_size, stream)){
        // if the input is too large
        if (strlen(input) > 513){ 
            print_error();
           // print_prompt();
            continue;    
	}
	is_internal = 0;
	is_redir = 0;			

	if(input[strlen(input) - 1] == '\n'){ 
	    input[strlen(input) - 1] = '\0';
	}
	//print previous command
	if (input[0] == '!') {
	    if (word_cnt == 1){
                int length = strlen(input);
		for(i = 0; i < length; i++){
		    input[i] = '\0';
		}
		if(length == 1){
		    int index = current - 1;
		    if(index < 0){
	                index += 20;
		    }
		  // input = (history[index]);
		    int temp = strlen(history[index]);
		    for(i = 0; i < temp; i++) {
	                input[i] = history[index][i];
		    }
		}   
		else{
		}
		is_internal = 1;
	   }   
	   else {
	       print_error();
	       print_prompt();
	   }
        }

        // redirection
        char *pre_redir = NULL;
        char *post_redir = NULL;
        tmp_input = strdup(input);
        pre_redir = strtok(tmp_input, ">");

        if (strlen(pre_redir) != strlen(input)){
            is_redir =1; 
            post_redir = strtok(NULL, " "); 
            if (strtok(NULL, " ")){ // check if too many input
                print_error();
                print_prompt();
                continue;
            }
        }
 
        // get word count 
         word_cnt = sep_line(input, words);
        
	//create history
	free(history[current]);
        history[current] = strdup(input);
        his_num++; 
        current = (current + 1) % History_Max;
        
        // check for redirection output file is valid or not
	int fd_out = 0;
	int dump = 0;
        if (is_redir){
	    dump = dup(STDOUT_FILENO);
	    close(STDOUT_FILENO);
            fd_out = open(post_redir, O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);   
            if (fd_out < 0){
                print_error();
                print_prompt();
                continue; 
            }
	} 
    
        // Internal command
        // exit
        char *str_exit = "exit";
        if (!strcmp(str_exit, input)){
            if (word_cnt != 1){
                print_error();
                print_prompt();
            }
            else{
		is_internal = 1;
		exit(0);
            }        
        }
       
	//print history
        char *str_history = "history";
	if (!strcmp(str_history, input)){
	    if (word_cnt != 1){
		print_error();
		print_prompt();
	    }
	    else{
	        is_internal = 1;
		print_history(history, current);
	    }
        }

        // fork and execvp, how to deal with fork and child is still undefined
        int child; 
        if (!is_internal){
            child = fork();    
            if (child == 0){
		char *command[512];
		int command_cnt = 0;
		if (is_redir){
		    for (i=0; i < word_cnt - 2; i++){
		        command[i] = strdup(words[i]);
		    }
		    command_cnt = i;
		    command[command_cnt] = NULL;
		}
		else{
	            for (i=0; i < word_cnt; i++){
		        command[i] = strdup(words[i]);
		    }
		command_cnt = i;
		command[command_cnt] = NULL;
		}
		execvp(command[0],command);
		print_error();
		exit(1);
	    }
	    else if(child > 0) {
                (void)wait(NULL);
	    }
	    else{
                print_error();
            }
	    if(is_redir){
            	close(fd_out);
	    	dup2(dump,STDOUT_FILENO);
	    	close(dump);
	    }
        }
     clear_words(words);
     print_prompt();
    }//end while loop
    clear_history(history);
    return 0;        
}








