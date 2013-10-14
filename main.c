
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
  char * cmd ; 
  struct node *next;
  int state;
};


void list_insert(char * str, pid_t  name, struct node ** head){
  struct node * newnode = (struct node * ) malloc (sizeof( struct node));
  newnode -> name = name ; 

  newnode -> cmd = str; 

  newnode -> next = NULL;
  
  newnode -> state = 1;
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


char * my_concat ( char * s1, char * s2)  {

  char * new_str = (char *) malloc (sizeof(strlen(s1)+ strlen(s2)-1 ));
  int count = 0;
  int count2 =0 ;
  int size_s1 = strlen(s1);
  int size_s2 = strlen(s2);
  while ( count < size_s1 -1){
    new_str[count]=  s1[count];
    count +=1;

  }while ( count < ((size_s1 + size_s2 ) -1)){
    new_str[count] = s2[count2];
    count +=1;
    count2 +=1;

  }
  new_str[count]='\0';


  return new_str;

}

void list_print (struct node *head){
  struct node * itr = head ;
  while ( itr != NULL){
    printf("\n Process ID: %d , The command: %s, State:%d \n ", itr -> name , itr-> cmd, itr-> state );
    itr = itr -> next ;
    printf("\n Inside loop \n ");
  }
}

void print_selected ( pid_t the_name, struct node * head){
  struct node * tmp = head;
  while (tmp !=NULL) {
    while (tmp-> name != the_name )
      {
	tmp = tmp -> next ;

      }if (tmp -> name == the_name ){break ;}

  }
  if(tmp ==NULL ){}
  else {
    printf ("\n Process %d (%s) completed \n", the_name, tmp -> cmd );

  }
}

void list_printer (  struct node * head ){
  struct node * tmp = head ; 
  while ( tmp != NULL){
    printf("the line is : %s", tmp -> cmd) ; 
    tmp = tmp -> next ;
  }
}

void pause_and_resume_selected ( pid_t the_name, struct node * head, int check){
  struct node * tmp = head;
  while (tmp !=NULL) {
    while (tmp-> name != the_name )
      {
	tmp = tmp -> next ;

      }if (tmp -> name == the_name ){break ;}

  }
  if(tmp ==NULL ){}
  else {
    
    if (check ){
      tmp -> state = 1 ;
      printf ("\n Proces %d  has been resumed \n", tmp -> name );} 
    else{ tmp -> state = 0; 
      printf ("\n Proces %d  has been paused \n", tmp ->name);} 
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

   int status ;

  printf("\n This is a prompt. Type 'exit' to get out! \n mode input is 'sequential' 's'  or   'parallel', 'p'  \n");
  FILE *  prog = stdin;

  const char * delim1 = ";"; //For each command
  const char* delim2 = "\n\t ";// For each component
  int mode = 0; // default to sequential 
  int current_mode = mode;
  int enter = 1;
  struct node * head = NULL ;
  struct node * the_paths = NULL;//****PATH RELATED 
  int path_file = 1;
  pid_t fake = 0; 

  int child_return_value;
  struct pollfd pfd[1];
  pfd -> fd = 0;
  pfd -> events = POLLIN;
  pfd -> revents = 0 ;
  struct node*   itr = NULL ;  
  const pid_t * kids_name = NULL; 
  //  char * ptr = NULL; **************************************Comment out ptr
  int for_exec = 1; 
  char* new_line = (char * ) malloc(sizeof(char)*1024);
  new_line =  fgets( new_line, 1024 ,  prog);

  //^^^^^^^^^^^^^^^^^^^^ATTEMP TO READ IN SHELL-CONFIG^^^^^^^^^^^^^^^^
  
  FILE *fp = fopen("shell-config","r");
  if (fp == NULL){ printf("\n shell-config doesn't exist. Please enter the full direct  ory path to a program to exectue \n");
    path_file = 0;
  }else {

    char * a_line = (char * ) malloc(sizeof(char)*1024);
     a_line =  fgets(a_line, 1024 , fp);
     while( a_line != NULL) {

       list_insert( a_line, fake, &the_paths  );
       
       a_line =  fgets( a_line, 1024 , fp);      
     }
  }
  //    list_printer( the_paths);
  
  
  //^^^^^^^^^^^^^^^^^^^^ATTEMP TO READ IN SHELL-CONFIG^^^^^^^^^^^^^^^^


  
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
	char * tmp3 = NULL;
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
	      printf("\n Enter a valid command. Did you mean mode ? \n" );
              cmd = strtok_r( NULL, delim1, &tmp);
	    }
	    continue;
	  }

        ///////////////////////////////////////EXIT FORMATING QUEUE COMPLETIONS\\\\\\\\\\\\\\\\

	if(strcmp(cmd_part,"exit") == 0){
	  enter = 0;
	  cmd = strtok_r(NULL,delim1, &tmp);
	  continue;
	  }
	//++++++++++++++++++++++++++++++JOBS BLOCK++++++++++++++++++++++++++++++++++++++++++
	//************************CALLING JOBS -Not Steve
	if(strcmp(cmd, "jobs") == 0){

	  list_print( head);
	  for_exec = 0;
	}
	printf("\n THE CMD IS %s \n", cmd );
	//**************************CALLING PAUSE 

	
	//        cmd_part = strtok_r (cmd, delim2, &tmp3 );
	if (strcmp(cmd_part,"pause")==0){
	  cmd_part = strtok_r (NULL, delim2, &tmp2);
	  printf ("\n i expect a pid : %s\n", cmd_part);
	  if ( cmd_part != NULL){

	    const char * input_cmd_part = (const char *) cmd_part;
	  int pid_for_pause = atoi(input_cmd_part);
	  kill(pid_for_pause, SIGSTOP);
	  pause_and_resume_selected ( pid_for_pause, head, 0);
	  for_exec = 0; }
	  else {printf("\n Please enter the name of the process. Eg, pause 234\n");}
	}  
	//**************************CALLING RESUME
        
	if (strcmp(cmd_part,"resume")==0){n
	  cmd_part = strtok_r (NULL, delim2, &tmp2 );
	  if ( cmd_part != NULL){
	    const char * input_cmd_part = (const char *) cmd_part;
	  int pid_for_pause = atoi(input_cmd_part);

	  kill(pid_for_pause, SIGCONT);
	  pause_and_resume_selected ( pid_for_pause, head, 1);
	  for_exec = 0; }
	  else {printf("\n Please enter the name of the process. Eg, resume 234\n");}
	}



	
	






	//++++++++++++++++++++++++++++++JOBS BLOCK++++++++++++++++++++++++++++++++++++++++++

     //!!!!!!!!!!!!!!!!!!!!!!!!!!!INPUT CMD CHECK !!!!!!!!!!!!!!!!!!!!!!!!!!!!
	char * c2 = NULL;
	struct stat statresult ;
	int rc = stat(the_cmd[0], &statresult);
	if (rc >= 0){
	  printf("\n THis command will be read just fine\n ");
	  for_exec = 1; 
	}
	else {//procced to combine the paths and the command to see if we can find the binary
	  struct node * path = the_paths ;
	  char * backslash = "//";
	  char * path_name = path -> cmd ; 
	  char * cmd_name = the_cmd[0];


	  while( path != NULL){
	    char * c1 = my_concat ( path_name , backslash);
	    //	    printf ("\n result is: %s\n", c1);	    
	     c2 = my_concat ( c1 ,cmd_name );
	    printf ("\n result is: %s\n", c2);	    
	    rc =  stat(c2 , &statresult);
        if (rc < 0){
	  printf("\n we couldn't find it in: %s", c2);
	  //MEANS the command given either doesn't include a path or isn't found in the path
	}else {printf("\nWe found the file in: %s\n", c2); break ;}
	    
	path = path -> next ;}
	

	  //--------------------constructing input for execv 
	//itr through the_cmd getting all the flags and adding them to the string you have
	//	char hold_all[100];
	//	char * hold_all_ptr = &hold_all;
	
	if( the_cmd[1]!=NULL && (c2!=NULL))  {//checking if there are flags 
	char* hold = my_concat (c2,the_cmd[1]); 
	
	int go = 2;
	while (the_cmd[go]!=NULL){
	  hold = my_concat (hold, the_cmd[go]);
	  go += 1 ;
	  
	  
	}
	printf ("\nhold: %s\n", hold);
	  }

	//---we now have the path to the binary and then the flags of the command in hold
	 the_cmd = tokenify (hold  );

	  //--------------------constructing input for execv 
	}

     //!!!!!!!!!!!!!!!!!!!!!!!!!!!INPUT CMD CHECK !!!!!!!!!!!!!!!!!!!!!!!!!!!!
	
    //------------------------------EXECV BLOCK----------------------------
	
     

     if ( current_mode == 0 && for_exec == 1){
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

	  

     if ( current_mode == 1 && for_exec == 1){
      //parallel 
       printf("OH NO YOU DIDN'");
        
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
	      list_insert(cmd,  pid, & head);
	      //waitpid( pid, &child_return_value ,0 ) ;
	    }
	  else {
	    printf("\n Fork failed\n");
	  }

 
     }
     freeArr(the_cmd);
     cmd = strtok_r( NULL, delim1, &tmp);
	}
//*******************************CHECKS FOR PROCESSES COMPLETED**********************8
      int rv = poll(&pfd, 1, 1000);

      if ( rv == 0)
	{// printf ("\n we just issued a stop 1");
	    struct node * ite = head;
	    //	    printf ("\n right after itr = head\n");
           while ( ite !=  NULL){
	     //	     printf("\n just enterd the while loop \n");
	     pid_t add_pid = ite -> name;
	     //	     printf("the name of the kid in the loop : %d", add_pid);
	     if (waitpid( add_pid, &child_return_value ,WNOHANG ) )
	       {//printf("\n Process %d (%s) \n", add_pid);
		 print_selected(add_pid, head);
	 	 kids_name = add_pid; 

		 list_delete( kids_name, & head);//remove the node that has the pid of the child that just compelted 


	       }  ite = ite -> next ;
	   }}
      else if (rv >  0)
	{//printf ("\n we just issued a stop 2 ");
	  struct node *ite = head;
           while ( ite !=  NULL){
	     pid_t add_pid = ite -> name;
	     
	     if (waitpid( add_pid, &child_return_value ,WNOHANG ) )
	       {printf("\n process pid: %d \n", add_pid);
	 	 kids_name = add_pid; 
		 ite = ite -> next ;
      	 	list_delete( kids_name, & head);//remove the node that has the pid of the child that just compelted 

	
	       }
	       else {
		 printf("\n MEANS CHILD ISN'T DONE \n");

	          }

	   }}

      else {printf("\n NOT WORKING \n");}
    

//*******************************CHECKS FOR PROCESSES COMPLETED**********************8



//New line prep
     current_mode = mode ;
     printf ("\n enter is: %d\n",enter);
     if(enter){
       new_line =  fgets( new_line, 1024 ,  prog);
     }
     //+++++++++++++++++++++checking if there are children finished +++++++++++++++++++++++++++
      }

  struct node * ite = head;
      while ( ite !=  NULL){
	pid_t add_pid = ite -> name;
	waitpid( add_pid, &child_return_value ,0 ) ;
	
	//process just completed 
        ite = ite -> next ;// ---advance pointer to the next node 


      }

    
    list_clear( head);
  

  
    fclose(prog);
    free(new_line);
    printf("\n Exiting \n");    
    return 0;



    }

