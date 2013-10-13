
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
//#include "list.h"

//LINKED LIST HELP 


struct node {
  pid_t name;
  struct node *next;
  
};


void list_insert(pid_t  name, struct node ** head){
  struct node * newnode = (struct node * ) malloc (sizeof( struct node));
  newnode -> name = name ;
  newnode -> next = NULL;
  struct node * tmp = * head ;

  if ( tmp == NULL)
    { newnode -> next = tmp;
      * head  = newnode ;
    }
  else {
    while ( tmp -> next != NULL){
      tmp = tmp -> next ;
    }
    tmp -> next = newnode ;
    newnode -> next = NULL;
  }
}

void list_delete(pid_t name,  struct node ** head ){
  struct node * tmp = * head ;
  //deleting the head node if it happens to match what we want
  if ( tmp -> name == name ){*head = tmp -> next ;}
  else {
    //loop until either you find the name or you're pointing to null
    while( ( tmp ->next -> name != name ) && (tmp !=NULL)){
      tmp = tmp -> next ;
    }
    if ( tmp == NULL){}//if you're pointing to nothing don't do anything
    else {//else make the current node point to the second node from it
      tmp -> next = tmp -> next -> next ;
    }
  }

}

void list_clear(struct node *list) {
  struct node *tmp=NULL;  
  while (list != NULL) {
    tmp = list->next;
    free(list);
    list = tmp;
  }
}

void list_print (struct node *head){
  struct node * itr = head ;
  while ( itr != NULL){
    printf("\n name is : %d \n ", itr -> name );
    itr = itr -> next ;
    printf("\n Inside loop \n ");
  }
}





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

void freeArr(char** array){
  int x = 0;
  char* tmp = NULL;
  while (array[x] != NULL){
    tmp = array[x];
    free(tmp);
    x++;
  }
  free(array[x]);
  free(array);
}


int main(int argc, char **argv) {
   char * s = "I think my dad's gone crazy ";
   char ** out_put = tokenify( s );
   int status ;
  printf( "\n the string: %s \n", out_put[2] );
  printf("\n This is a prompt. Type 'exit' to get out! \n mode input is 'sequential' 's'  or   'parallel', 'p'  \n");
  FILE *  prog = stdin;

  const char * delim1 = ";"; //For each command
  const char* delim2 = "\n\t ";// For each component
  int mode = 0; // default to sequential 
  int current_mode = mode;
  int enter = 1;
  struct node * head = NULL ;
  int child_return_value;
  struct pollfd pfd[1];
  pfd -> fd = 0;
  pfd -> events = POLLIN;
  pfd -> revents = 0 ;
  struct node*   itr = NULL ;  
  const pid_t * kids_name = NULL; 
  //  char * ptr = NULL; **************************************Comment out ptr

  char* new_line = (char * ) malloc(sizeof(char)*1024);
  new_line =  fgets( new_line, 1024 ,  prog);
  
  //while (1) //We always read in a new line.
  while(enter)

    {

      char * comment = strchr(new_line, '#'); //deal with comment on line
      if(comment != NULL){
	* comment = '\0';
      } ;
      
      
      //       printf ("\n command at this point  %d \n ", cmd);


      char * tmp  = NULL;
      char* cmd = strtok_r( new_line, delim1, &tmp);


      while(cmd != NULL){
	// as new job come in, check the parse for exit or mode

	char* tmp2 = NULL;
	/*
// This block makes the space before the pound go away but this 
//	   messes up our sleep because it cuts off the last argument (aka the numbe//r)
	  printf("\ntest command: %s \n", cmd);
	
	//*/
	
	char ** the_cmd = tokenify ( cmd );
	char * cmd_part =strtok_r(cmd, delim2, &tmp2); //Getting the command

	if(cmd_part == NULL){ //cmd is only whitespace
	  cmd = NULL;
	  continue;
		
	}

	 if(strcmp(cmd_part, "mode") == 0)
	  {  printf("\n ENTERED MODE \n");
	    cmd_part = strtok_r( NULL, delim2, &tmp2);
	    if ( cmd_part == NULL ){
	        cmd = strtok_r( NULL, delim1, &tmp);
		if( current_mode == 1) {printf("\n You are running in Parallel mode \n");}
		else{ printf("\n You are running in Sequential mode \n");}

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
	    continue;
	  }

        //EXIT FORMATING QUEUE COMPLETIONS 

	if(strcmp(cmd_part,"exit") == 0){
	  enter = 0;
	  cmd = strtok_r(NULL,delim1, &tmp);
	  continue;
	  }
	
	
    //------------------------------EXECV BLOCK----------------------------
	
     

     if ( current_mode == 0){
       //Sequential
       printf ("\n ENTERED SEQUENTIAL \n");
       //       ptr = & the_cmd;***********************************comment out ptr 
	  pid_t pid;
	  //	  int child_return_value;

	  pid = fork();
	  if (pid == 0 )//checks if we have the child
	    {
	      if (execv(the_cmd[0],  the_cmd) < 0) {
	       fprintf(stderr, "execv failed: %s\n", strerror(errno));

	      }	       return 1;
	    }

	  else if  ( pid > 0) //checks if we have a parent
         	    {
	      waitpid( pid, &child_return_value ,0 ) ;

	    } 
	  else {
	    printf("\n Fork failed\n");
	  }
     }

	  

    else if (current_mode == 1){
      //parallel 

        
      //          char * ptr = & the_cmd; ********************************Commment out ptr
	  pid_t pid;
	  //	  int child_return_value;
	  
	  pid = fork();
	  
	  //	  printf("\n
	  if (pid == 0 )//checks if we have the child
	    { //printf ("we have a kid by the number: %d", pid);
	      if (execv(the_cmd[0],  the_cmd) < 0) {
	       fprintf(stderr, "execv failed: %s\n", strerror(errno));
	      }
	       return 1;	      
	    }
	  
	  else if  ( pid > 0) //checks if we have a parent
	    { 
	      list_insert( pid, & head);
	      //waitpid( pid, &child_return_value ,0 ) ;
	    }
	  else {
	    printf("\n Fork failed\n");
	  }

 
     }
     freeArr(the_cmd);
     cmd = strtok_r( NULL, delim1, &tmp);
      }
      int rv = poll(&pfd, 1, 1000);
      if ( current_mode ==1 ){
      if ( rv == 0)
	{ printf ("\n we just issued a stop 1");
	    itr = head;
	    //	    printf ("\n right after itr = head\n");
           while ( itr !=  NULL){
	     //	     printf("\n just enterd the while loop \n");
	     pid_t add_pid = itr -> name;
	     //	     printf("the name of the kid in the loop : %d", add_pid);
	     if (waitpid( add_pid, &child_return_value ,WNOHANG ) )
	       {printf("\n process pid: %d \n", add_pid);
	 	 kids_name = add_pid; 
     		 itr = itr -> next ;
		 list_delete( kids_name, & head);//remove the node that has the pid of the child that just compelted 


	       }}}
      else if (rv >  0)
	{//printf ("\n we just issued a stop 2 ");
	  itr = head;
           while ( itr !=  NULL){
	     pid_t add_pid = itr -> name;
	     
	     if (waitpid( add_pid, &child_return_value ,WNOHANG ) )
	       {printf("\n process pid: %d \n", add_pid);
	 	 kids_name = add_pid; 
     		 itr = itr -> next ;
      	 	list_delete( kids_name, & head);//remove the node that has the pid of the child that just compelted 

	
	       }
	       else {
     		 itr = itr -> next ;
	          }

	   }}

      else {printf("\n NOT WORKING \n");}
      }
    //----------Waiting for our children to die -------------


//New line prep
     current_mode = mode ;
     if(enter){
       new_line =  fgets( new_line, 1024 ,  prog);
     }
     //+++++++++++++++++++++checking if there are children finished +++++++++++++++++++++++++++

    }
      itr = head;
      while ( itr !=  NULL){
	pid_t add_pid = itr -> name;
	waitpid( add_pid, &child_return_value ,0 ) ;
	
	//process just completed 
        itr = itr -> next ;// ---advance pointer to the next node 


        }
    list_clear( head);
  

  
    fclose(prog);
    free(new_line);
    printf("\n Exiting \n");    
    return 0;
	}



