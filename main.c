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
  prog = stdin;


  const char* delim = "\n\t ;";
  int mode = 0; // default to sequential 
   
  char* new_line = (char * ) malloc(sizeof(char)*1024);
  new_line =  fgets( new_line, 1024 ,  prog);

  while (1) //We always read in a new line.
    {
      char * tmp= NULL;
      char * new_parse = strtok_r( new_line, delim, &tmp);

      while(1){ //always parse. I guess breaks will happen manually so far
	// as new job come in, check the parse for exit or mode


	if(strcmp(new_parse, "mode") == 0)
	  {
	    new_parse = strtok_r( new_line, delim, &tmp);
	    if(new_parse[0]=='s'){
	      mode = 0;
	      new_parse = strtok_r( new_line, delim, &tmp);
	    };
	    else(){
		mode = 1;
		new_parse = strtok_r( new_line, delim, &tmp);
	      };
	    continue; // run while loop again
	  };


	if(strcmp(new_parse,"exit") == 0){
	    // DOSTUFF and break;break;
	  };


      }
      if(strcmp(new_parse) == 0 ){
	//here, finish all jobs, and then exit program

      };

    }


  fclose(prog);

  printf("\n You entered the word 'exit'. Peace out, yo!")
    return 0;
}

