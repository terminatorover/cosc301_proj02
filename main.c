
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <poll.h>
#include <signal.h>


 int main(int argc, char **argv) {
  printf("\n This is a prompt. Type 'exit' to get out! \n mode input is 'sequential', 's'  or 'parallel', 'p'  \n");
  FILE *  prog = stdin;

  const char * delim1 = ";\0"; //For each command
  const char* delim2 = "\n\t ";// For each component
  int mode = 0; // default to sequential 
  int current_mode = mode;
  
  char* new_line = (char * ) malloc(sizeof(char)*1024);
  new_line =  fgets( new_line, 1024 ,  prog);

  //  while (1) //We always read in a new line.
  //{
      char * comment = strchr(new_line, '#'); //deal with comment on line
      if(comment != NULL){
	* comment = '\0';
      } ;

      char * tmp= NULL;
      char * cmd = strtok_r( new_line, delim1, &tmp);

      while(cmd!= NULL){ //always parse. I guess breaks will happen manually so far
	// as new job come in, check the parse for exit or mode

	char* tmp2 = NULL;
	char * cmd_part =strtok_r(cmd, delim2, &tmp2); 
	printf("cmd is %s ", cmd);
	//MODE FORMATING OPTIONS

	if(strcmp(cmd_part, "mode") == 0)
	  {  
	    cmd_part = strtok_r( NULL, delim2, &tmp2);
	    if ( cmd_part == NULL ){
	        cmd = strtok_r( NULL, delim1, &tmp);
	        printf("\n You are running in %d mode.\n" , mode );
	    }
	    else if(cmd_part[0]=='p'){
	      mode = 1;
	      cmd = strtok_r( NULL, delim1, &tmp);
	    }
	    else if(cmd_part[0]=='s'){
		mode = 0;
		cmd = strtok_r( NULL, delim1, &tmp);
	      }
	    else{
	      printf("\n You are running in %d mode.\n" , mode );
              cmd = strtok_r( NULL, delim1, &tmp);
	    }
	    //    continue; // run while loop again
	  }

	//	 printf("\nchanging ? %s \n", cmd);

	 //EXIT FORMATING QUEUE COMPLETIONS 

	if(strcmp(cmd_part,"exit") == 0){
           // DOSTUFF and break;break;
	  
	  }

	//EXECV BLOCK

	if ( current_mode == 0){
	  // sequential 
	  pid_t pid;
	  int child_return_value;
	  pid = fork();
	  if (pid == 0 )//checks if we have the child
	    {
	      if (execv(cmd[0], cmd) < 0) {
		fprintf(stderr, "execv failed: %s\n", strerror(errno));
	      }
	    }

	  else if  ( pid > 0) //checks if we have a parent
	    {
	      waitpid( pid, &child_return_value ,0 ) ;
	      printf("\nChild process finsished\n");
	    }
	  else {
	    printf("\n Fork failed\n");
	  }

	}
	else if (current_mode == 1){
	  //parallel 
	  
	}

	cmd = strtok_r( NULL, delim1, &tmp);
	 printf("\nchanging ? %s \n", cmd);





      }
      //}


  fclose(prog);

  printf("\n You entered the word 'exit'. Peace out, yo!");
    return 0;
}

