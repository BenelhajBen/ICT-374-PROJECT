#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "token.h"

int tokenise(char *inputline, char* token[])
{

   char* tok;
   int i = 0;
   
   tok = strtok(inputline, seperator);
   token[i] = tok;
   while(tok!=NULL){
      i++;
      if(i >= MAX_NUM_TOKENS){
         return -1;
      }
      
      tok = strtok(NULL, seperator);
      token[i] = tok;
   }
   return i;
   
   

}
