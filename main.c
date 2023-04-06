#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <glob.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include "include/token.h"
#include "include/commandexecution.h"

#define INPUT_SIZE 1024
void ignore_signal(int signum);
void sigchld_handler(int signum);
void print_menu();

int main()
{

    char prompt[PROMPT_SIZE] = "$ ";
    char input[INPUT_SIZE];
    char *tokens[MAX_NUM_TOKENS];
    Command cmnd[MAX_NUM_COMMANDS];

    signal(SIGINT, ignore_signal);  // ctrlc
    signal(SIGQUIT, ignore_signal); // ctrl\(sigquit)
    signal(SIGTSTP, ignore_signal); // ctrlz
    signal(SIGCHLD, sigchld_handler);
    print_menu();
    while (1)
    {
        int status;
        while (waitpid(-1, &status, WNOHANG) > 0); 
        fflush(stdout); // Flush the output buffer
        printf("%s ", prompt);
        int again = 1;
        char *linept; // pointer to the line buffer

        while (again)
        {
            again = 0;
            linept = fgets(input, INPUT_SIZE, stdin);
            if (linept == NULL)
            {
                if (errno == EINTR)
                {
                    again = 1;
                }
                else
                {
                    // Handle EOF (Ctrl + D)
                    printf("\n");
                    exit(0);
                }
            }
        }

        // Tokenize input and separate commands
        tokenise(input, tokens);
        int numCommands = separateCommands(tokens, cmnd);
        for (int i = 0; i < numCommands; i++)
        {

            char *cmd_name = cmnd[i].argv[0];
            int command_handle = 0;
            if (strcmp(cmd_name, "prompt") == 0)
            {
                prompt_handler(prompt);
            }
            else if (strcmp(cmd_name, "pwd") == 0)
            {
                pwd_handler();
            }
            else if (strcmp(cmd_name, "exit") == 0 || strcmp(cmd_name, "quit") == 0)
            {

                exit(0);
            }
            else if (strcmp(cmd_name, "cd") == 0)
            {
                if (cmnd[i].argv[1])
                {
                    cd_handler(cmnd[i].argv[1]);
                }
                else
                {
                    // Handle the case where no argument is provided to cd
                    char *home = getenv("HOME");
                    if (home)
                    {
                        cd_handler(home);
                    }
                    else
                    {
                        fprintf(stderr, "cd: HOME environment variable not set.\n");
                    }
                }
            }
            else if (strcmp(cmd_name, "ls") == 0)
            {
                int wildcard_present = 0;
                glob_t globbuf;

                for (int j = 1; cmnd[i].argv[j] != NULL; j++)
                {
                    if (strchr(cmnd[i].argv[j], '*') != NULL || strchr(cmnd[i].argv[j], '?') != NULL)
                    {
                        wildcard_present = 1;
                        if (glob(cmnd[i].argv[j], GLOB_DOOFFS, NULL, &globbuf) == GLOB_NOMATCH)
                        {
                            printf("No files matching %s\n", cmnd[i].argv[j]);
                        }
                        else
                        {
                            int glob_index = 0;
                            while (globbuf.gl_pathv[glob_index])
                            {
                                // Copy the matched path into the argument list
                                cmnd[i].argv[j] = globbuf.gl_pathv[glob_index];
                                execute_command(&cmnd[i]);
                                ++glob_index;
                            }
                            globfree(&globbuf);
                        }
                    }
                }

                if (!wildcard_present)
                {
                    execute_command(&cmnd[i]);
                }
            }
            else
            {

                int numPipes = 0;
                for (int j = i; j < numCommands - 1 && strcmp(cmnd[j].sep, "|") == 0; ++j)
                {
                    numPipes++;
                }

                if (numPipes > 0)
                {
                    execute_pipe(cmnd, i, numPipes);
                    i += numPipes; // Skip over the piped commands that were already executed
                }
                else
                {
                    execute_command(&cmnd[i]);
                }
            }
        }
    }
    return 0;
}

void print_menu()
{
    printf("\033[H\033[J"); // Clear the screen
    printf("Shell Menu:\n");
    printf("1. cd [directory] - Change the current directory to the specified directory.\n");
    printf("2. pwd - Print the current working directory.\n");
    printf("3. ls [options] [files/directories] - List files and directories.\n");
    printf("4. exit - Exit the shell.\n");
    printf("\nType a command followed by its arguments, if any.\n");
    printf("\n");
}

void ignore_signal(int signum){};

void sigchld_handler(int signum)
{
    while (waitpid(-1, NULL, WNOHANG) > 0)
        ;
}
