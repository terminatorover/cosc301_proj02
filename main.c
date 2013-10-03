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

  const char * delim1 = ";"; //For each command
  const char* delim2 = "\n\t ";// For each component
  int mode = 0; // default to sequential 
   
  char* new_line = (char * ) malloc(sizeof(char)*1024);
  new_line =  fgets( new_line, 1024 ,  prog);

  //  while (1) //We always read in a new line.
  //{
      char * comment = strchr(new_parse, '#'); //deal with comment on line
      if(comment != NULL){
	comment = '\0';
      } ;

      char * tmp= NULL;
      char * new_parse = strtok_r( new_line, delim1, &tmp);

      while(1){ //always parse. I guess breaks will happen manually so far
	// as new job come in, check the parse for exit or mode

	char* tmp2 = NULL;
	char * new_parse_2 =strtok_r(new_parse, delim2, &tmp2); 
	if(strcmp(new_parse_2, "mode") == 0)
	  {
	    new_parse_2 = strtok_r( new_line, delim2, &tmp2);
	    if(new_parse_2[0]=='p'){
	      mode = 1;
	      new_parse = strtok_r( new_line, delim1, &tmp);
	    };
	    else if(new_parse_2[0]=='s'){
		mode = 0;
		new_parse = strtok_r( new_line, delim1, &tmp);
	      };
	    else{
	      printf("\n You are running in %d mode.\n" , mode );
	    };
	    continue; // run while loop again
	  };


	if(strcmp(new_parse_2,"exit") == 0){
           // DOSTUFF and break;break;
	  };


      }
      //}


  fclose(prog);

  printf("\n You entered the word 'exit'. Peace out, yo!")
    return 0;
}

ls -l ; rm -rf abc.txt  ; mode sequential ; rm ab
