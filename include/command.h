// file:		command.h for Week 8
// purpose;		to separate a list of tokens into a sequence of commands.
// assumptions:		any two successive commands in the list of tokens are separated 
//			by one of the following command separators:
//				"|"  - pipe to the next command
//				"&"  - shell does not wait for the proceeding command to terminate
//				";"  - shell waits for the proceeding command to terminate
// author:		HX
// date:		2006.09.21
// last modified:	2006.10.05
// note:		not tested therefore it may contain errors
#ifndef COMMAND_H
#define COMMAND_H
#define MAX_NUM_COMMANDS  2048
#define PROMPT_SIZE 100
// command separators
#define pipeSep  "|"                            // pipe separator "|"
#define conSep   "&"                            // concurrent execution separator "&"
#define seqSep   ";"                            // sequential execution separator ";"
#define MAX_PIPES 1000
struct CommandStruct {
   int first;      // index to the first token in the array "token" of the command
   int last;       // index to the first token in the array "token" of the command
   int argc;
   char *sep;	   // the command separator that follows the command,  must be one of "|", "&", and ";"
   char **argv;        // an array of tokens that forms a command
   char *stdin_file;   // if not NULL, points to the file name 
                        // for stdin redirection                        
   char *stdout_file;  // if not NULL, points to the file name 
                        // for stdout redirection
   char com_suffix;
};

typedef struct CommandStruct Command;  // command type
int separateCommands(char *token[], Command command[]);
void searchRedirection(char *token[], Command *cp); 
void buildCommandArgumentArray(char *token[], Command *cp); 

#endif /*COMMAND_h*/
