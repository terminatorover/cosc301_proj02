

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
#include <errno.h>
//HELPER
char** tokenify(const char *str) {
  const char *sep = " \n\t";
  char *temp, *word;
  char *s = strdup(str);
  int counter = 0;
  for (word = strtok_r(s, sep, &temp); word != NULL; word = strtok_r(NULL, sep, &temp)) {
    counter++;
  }
  free(s);
  counter++;
  char **array = (char**)malloc(sizeof(char*) * counter);
  int x = 0;
  char *temp1, *word1;
  char *s1 = strdup(str);
  
  for (word1 = strtok_r(s1, sep, &temp1); word1 != NULL; word1 = strtok_r(NULL, sep, &temp1)) {
    array[x]= strdup(word1);
    x++;
  }
  free(s1);
  array[x]=NULL;
  return array;
  
}



int main(int argc, char **argv) {
   char * s = "I think my dad's gone crazy ";
   char ** out_put = tokenify( s );

  printf( "\n the string: %s \n", out_put[2] );
  printf("\n This is a prompt. Type 'exit' to get out! \n mode input is 'sequential' 's'  or 'parallel', 'p'  \n");
  FILE *  prog = stdin;

  const char * delim1 = ";\0"; //For each command
  const char* delim2 = "\n\t ";// For each component
  int mode = 0; // default to sequential 
  int current_mode = mode;
  
  char* new_line = (char * ) malloc(sizeof(char)*1024);
  new_line =  fgets( new_line, 1024 ,  prog);

  //  while (1) //We always read in a new line.
  //{
  printf(" \n this is newline: %s \n", new_line);
      char * comment = strchr(new_line, '#'); //deal with comment on line
      if(comment != NULL){
	* comment = '\0';
      } ;
  printf(" \n this is newline: %s \n", new_line);
      char * tmp= NULL;
      char * cmd = strtok_r( new_line, delim1, &tmp);

      //       printf ("\n command at this point  %d \n ", cmd);
    while(cmd != NULL){ //always parse. I guess breaks will happen manually so far
	// as new job come in, check the parse for exit or mode
            printf ("\n command at this point  %s \n ", cmd);
	char* tmp2 = NULL;
       char ** the_cmd = tokenify ( cmd );
       
       //       printf("\n indexing ??? maybe  %s \n", the_cmd[1]);
       char * my_cmd = *the_cmd ;

	char * cmd_part =strtok_r(cmd, delim2, &tmp2); //Getting the command


	if(strcmp(cmd_part, "mode") == 0)
	  {  printf("\n ENTERED HELL \n");
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
            else{// shouldn't we display some type of error message. 
	//the user typed mode and then ???
	      printf("\n You are running in %d mode.\n" , mode );
              cmd = strtok_r( NULL, delim1, &tmp);
	    }
	  }
	    //    continue; // run while loop again
	  
	//shouldn't we at some point change the value of current_mode

	//	 printf("\nchanging ? %s \n", cmd);

        //EXIT FORMATING QUEUE COMPLETIONS 

	if(strcmp(cmd_part,"exit") == 0){
           // DOSTUFF and break;break;
	  
	  }
	
	
    //------------------------------EXECV BLOCK----------------------------
	
	

     if ( current_mode == 0){
       //Sequential
       char * ptr = & the_cmd;
	  pid_t pid;
	  int child_return_value;

	  pid = fork();
	  if (pid == 0 )//checks if we have the child
	    {
	      if (execv(the_cmd[0],  the_cmd) < 0) {
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
             char * ptr = & the_cmd;
	  pid_t pid;
	  int child_return_value;

	  pid = fork();
	  if (pid == 0 )//checks if we have the child
	    {
	      if (execv(the_cmd[0],  the_cmd) < 0) {
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
     
    

     
	cmd = strtok_r( NULL, delim1, &tmp);
    
  

 

    }
//New line prep
     current_mode = mode ;

    
    fclose(prog);
    return 0;
    }



