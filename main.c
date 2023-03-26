#include <stdio.h>
#include <stdlib.h>
#include "command.h"
#include "token.h"


#define MAX_INPUT_LENGTH 1000

void printInitialScreen();

int main() {
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    
    char input[MAX_INPUT_LENGTH];
    
    char *tokenArray[MAX_INPUT_LENGTH];
    
    pid_t pid;
    int status;
    int more = 1;
    int loop = 1;
    printInitialScreen();
    while(loop){
       pid_t pid2;
       int p[2];
       int np = 0;
    
    }
    
}

void printComStruct(struct Command_struct *com)
{ 
  int i;

  fprintf(stderr,"com_pathname=%s\n", com->com_pathname); 
  fprintf(stderr,"argc=%d\n", com->argc); 
  for(i=0;  com->argv[i]!=NULL; i++) 
      fprintf(stderr,"argv[%d]=%s\n", i, com->argv[i]); 
      fprintf(stderr,"#######\n"); 
      if (com->redirect_in == NULL)
          fprintf(stderr,"redirect_in=NULL\n"); 
      else
          fprintf(stderr,"redirect_in=%s\n", com->redirect_in); 
      if (com->redirect_out == NULL)
          fprintf(stderr,"redirect_out=NULL\n"); 
      else
          fprintf(stderr,"redirect_out=%s\n", com->redirect_out); 
      fprintf(stderr,"com_suffix=%c\n\n", com->com_suffix); 
} 