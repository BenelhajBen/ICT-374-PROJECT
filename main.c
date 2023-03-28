#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "command.h"
#include <signal.h>
#include <string.h>
#include <glob.h>


#define PROMPT_SIZE 1000
#define INPUT_SIZE 1024
void ignore_signal(int signum);
void sigchld_handler(int signum);
void printInitialScreen();

int main() {

    char prompt[PROMPT_SIZE] = "$ ";
    char input[INPUT_SIZE];
    char *tokens[MAX_NUM_TOKENS];
    Command cmnd[MAX_NUM_COMMANDS];
    
    signal(SIGINT, ignore_signal); // ctrlc
    signal(SIGQUIT, ignore_signal); // ctrl\(sigquit)
    signal(SIGTSTP, ignore_signal); // ctrlz
    signal(SIGCHLD, sigchld_handler);
    while (1) {
        printf("%s", prompt);
        fgets(input, INPUT_SIZE, stdin);
        int fd[2] = {-1,-1};
        int fd_in = 0;

        // Tokenize input and separate commands
        
        tokenise(input, tokens);
        int numCommands = separateCommands(tokens, cmnd);

        for (int i = 0; i < numCommands; i++) {
            char *cmd_name = cmnd[i].argv[0];
		
            if (strcmp(cmd_name, "prompt") == 0) {
                prompt_handler(prompt);
            } else if (strcmp(cmd_name, "pwd") == 0) {
                pwd_handler();
            } else if (strcmp(cmd_name, "exit") == 0) {
                exit(0);
            } else if (strcmp(cmd_name, "cd") == 0) {
                if (cmnd[i].argv[1]) {
                    cd_handler(cmnd[i].argv[1]);
                } else {
                    // Handle the case where no argument is provided to cd
                    char *home = getenv("HOME");
                    if (home) {
                        cd_handler(home);
                    } else {
                        fprintf(stderr, "cd: HOME environment variable not set.\n");
                    }
                }
                continue;
            }else if(strcmp(cmnd[i].argv[0], "ls") == 0) {
		int has_wildcards = 0;
		for (int j = 1; cmnd[i].argv[j] != NULL; j++) {
		   if (strchr(cmnd[i].argv[j], '*') != NULL || strchr(cmnd[i].argv[j], '?') != NULL) {
			    has_wildcards = 1;
			    glob_t globbuf;
			    if (glob(cmnd[i].argv[j], GLOB_DOOFFS, NULL, &globbuf) == GLOB_NOMATCH) {
				printf("No files matching %s\n", cmnd[i].argv[j]);
			    } else {
				int k = 0;
				while(globbuf.gl_pathv[k]) {
				    printf("%s\n", globbuf.gl_pathv[k]);
				    ++k;
				}
			    }
			    globfree(&globbuf);
			}
	        }

		if (!has_wildcards) {
			execute_command(&cmnd[i]);
		}
            }

            pid_t pid = fork();
            if (pid == 0) {
                // In the child process

                // Redirect input if necessary
                if (fd_in != 0) {
                    dup2(fd_in, cmnd[i].stdin_file);
                    close(fd_in);
                }

                // Redirect output if necessary
                if (strcmp(cmnd[i].sep, pipeSep) == 0) {
                    dup2(fd[1], cmnd[i].stdout_file);
                    close(fd[1]);
                }

                execute_command(&cmnd[i]);
                exit(0);
            } else if (pid > 0) {
                // In the parent process

                // Close the write end of the pipe if necessary
                if (strcmp(cmnd[i].sep, pipeSep) == 0) {
                    close(fd[1]);
                }

                // Update the input file descriptor for the next command if necessary
                if (fd_in != 0) {
		    dup2(fd_in, 0); // Replace cmnd->stdin_file with 0
		    close(fd_in);
		}

		// Redirect output if necessary
		if (strcmp(cmnd[i].sep, pipeSep) == 0) {
		    dup2(fd[1], 1); // Replace cmnd->stdout_file with 1
		    close(fd[1]);
		}

                if (strcmp(cmnd[i].sep, conSep) != 0) {
                    // If the command is not followed by an "&", wait for it to finish
                    int status;
                    waitpid(pid, &status, 0);
                }
            } else {
                // Handle fork error
                perror("fork");
            }

        }
    }

    return 0;
}

void ignore_signal(int signum){ };

void sigchld_handler(int signum){
  while(waitpid(-1,NULL, WNOHANG) > 0);

}



