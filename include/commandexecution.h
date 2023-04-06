#include "../include/command.h"
void execute_command(Command* cmd);
void globber(char *token, char ***filearray);
void prompt_handler(char *prompt);
void pwd_handler();
void cd_handler();
void exit_handler();
void execute_pipe(Command *cmd, int startIndex, int numPipes);