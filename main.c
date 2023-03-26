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

void prompt(char* prmpt)
{
  printf("%s", prmpt);
}
