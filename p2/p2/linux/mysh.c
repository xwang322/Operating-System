#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>

#define History_Max 20  

//define the global variables
char prompt_message[7] = "mysh # ";
char error_message[30] = "An error has occurred\n";
int is_batch = 0;
int word_cnt = 0;
unsigned int his_num = 0;  

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
  char *words[512*2];
  char *tempWords[512*2];
  int is_internal = 0;
  int is_redir = 0;
  char *tmp_input = NULL;
  char input[512*2];
  char *history[History_Max];  
  int i;
  int current = 0;
  for(i = 0; i < History_Max; i++){
    history[i] = NULL;    
  }   
  // Judge a file is a batch or not
  if (argc == 1){
    stream = stdin;  
  }
  else if (argc == 2){ 
    stream = fopen(argv[1], "r");
    if (stream == NULL){
      print_error();
      exit(1);
    }
    is_batch = 1;
  }
  else{
    print_error();
    exit(1);
  }
  print_prompt();
  // Tell whether it is a valid input command
  while (fgets(input, 512*2, stream)){
    if (strlen(input) > 513){  
      input[512]='\n';
      write(STDOUT_FILENO, input, 513);
      print_error();
      continue;    
    }
    if (is_batch) {
      write(STDOUT_FILENO, input, strlen(input));
    }
    if (sep_line(input, tempWords) == 0){
      print_prompt();
      continue;
    }
    is_internal = 0;
    is_redir = 0;			
    word_cnt = sep_line(input, words);

      //print previous command
      if (words[0][0] == '!'){
	if (word_cnt == 1){
	  int length = strlen(words[0]);
	  int k;
	  for (k = 0; k < length; k++){
	    input[k] = '\0';
	  }
	  if (length == 1){
	    int index = current - 1;
	    if (index <= 0){
	      index += 20;
	    }
	    // input = (history[index]);
	    int temp = strlen(history[index]);
	    for (i = 0; i < temp; i++){
	      input[i] = history[index][i];
	    }
	  }
	  else{
             char *temp = NULL;
             char *his_temp = NULL;
             int i, num;
             temp = strtok(words[0], "!");
             num = atoi(temp);
             num = num % 20;
             int index = current - 1;
	     if (index <= 0){
	       index += 20;
	     }
             if (num > his_num){
               print_error();
               exit(0);
             }
             if (his_num > 20){
               if (num < his_num-19){
                 print_error();
                 exit(0);
               }
             }
             his_temp = history[num-1];
             strcpy(input, his_temp);
	   }
	   is_internal = 0;
    	}
        else {
	  print_error();
	  print_prompt();
          exit(0);
	}
      }

        // redirection
      char *pre_redir = NULL;
      char *post_redir = NULL;
      tmp_input = strdup(input);
      pre_redir = strtok(tmp_input, ">");
      if (strlen(pre_redir) != strlen(input)){
        is_redir =1; 
        post_redir = strtok(NULL, ">"); 
        if (strtok(NULL, ">")){ 
          print_error();
          print_prompt();
          continue;
        }
      }

        // get word count 
      if (!is_redir){
        word_cnt = sep_line(input, words);
      }
      else {
	word_cnt = sep_line(pre_redir, words);
	int extra = sep_line(post_redir, &(words[word_cnt]));
	word_cnt += extra;
	if (extra != 1){
	  print_error();
	  print_prompt();
	  continue;
	}
      }

      if(input[strlen(input) - 1] == '\n'){ 
	input[strlen(input) - 1] = '\0';
      }
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
        fd_out = open(words[word_cnt-1], O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU); 
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
          continue;
        }
        else {
	  exit(0);
        }        
      }
       
	//print history
      char *str_history = "history";
      if (!strcmp(str_history, input)){
        if (word_cnt != 1){
	  print_error();
	  print_prompt();
	  continue;
	}
	else {
	  is_internal = 1;
	  print_history(history, current);
	}
      }
		
      char *command[512*2];
      int command_cnt = 0;
      if (is_redir){
	for (i = 0; i < word_cnt - 1; i++){
	  command[i] = strdup(words[i]);
	}
	command_cnt = i;
	command[command_cnt] = NULL;
      }
      else {
	for (i = 0; i < word_cnt; i++){
	  command[i] = strdup(words[i]);
	}
	command_cnt = i;
	command[command_cnt] = NULL;
      }
     
      pid_t child; 
      if (!is_internal){
        child = fork();    
        if (child == 0){
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








