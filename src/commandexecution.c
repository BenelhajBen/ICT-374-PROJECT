#include "../include/commandexecution.h"
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <limits.h>

void sigchild_handler(int sig)
{
    while (waitpid(-1, NULL, WNOHANG) > 0)
    {
    }
}
void execute_command(Command *command)
{
    // variables for process Id, output file description, and input file descriptor.
    pid_t pid;
    int ofd = -1; // stdout redirection
    int ifd = -1; // stdin redirection

    // Fork a new process.
    if ((pid = fork()) < 0)
    {
        perror("fork\n");
        exit(1);
    }

    if (pid == 0) // child
    {
        // Handle stdout redirection if required
        if (command->stdout_file != NULL)
        {
            // Open the file for writing and create if it doesnt exist.
            ofd = open(command->stdout_file, O_CREAT | O_WRONLY | O_TRUNC, 0666);
            if (ofd < 0)
            {
                perror("open");
                return;
            }
            dup2(ofd, STDOUT_FILENO);
            close(ofd);
        }

        // Handle stdin redirection if needed
        if (command->stdin_file != NULL)
        {
            // open file for reading
            ifd = open(command->stdin_file, O_RDONLY);
            if (ifd < 0)
            {
                perror("open");
                return;
            }
            dup2(ifd, STDIN_FILENO);
            close(ifd);
        }

        // Execute command
        if (execvp(command->argv[0], command->argv) < 0)
        {
            printf("Command '%s' failed\n", command->argv[0]);
            exit(1);
        }
    }
    else // parent
    {
        // Handle the separators
        if (strcmp(command->sep, seqSep) == 0 || strcmp(command->sep, pipeSep) == 0)
        {
            // waiting for the child process to finish
            wait(NULL);
        }
        else if (strcmp(command->sep, conSep) == 0)
        {
            // Prints the background process for debugging process info
            // printf("\n[%d] Background process \n\n", pid);
            struct sigaction sa;
            sa.sa_handler = sigchild_handler;
            sigemptyset(&sa.sa_mask);
            if (sigaction(SIGCHLD, &sa, NULL) == -1)
            {
                perror("sigaction");
                exit(1);
            }
        }
        else
        {
            // wait for the child process when the separator is not &
            wait(NULL);
        }
    }
    return;
}

void execute_pipe(Command *cmd, int startIndex, int numPipes)
{
    // Array of pipes.
    int pipefd[numPipes][2];

    // Create required number of pipes.
    for (int i = 0; i < numPipes; i++)
    {
        if (pipe(pipefd[i]) < 0)
        {
            perror("Error in piping\n");
            return;
        }
    }
    
    // iterating through each command within the pipe line.
    for (int i = 0; i <= numPipes; i++)
    {
        // forking a new process.
        pid_t pid = fork();

        if (pid < 0)
        {
            perror("Error in forking\n");
            return;
        }

        if (pid == 0) // child process
        {
            // if not last command duplicate the write end of the current pipe to STDOUT
            if (i < numPipes)
            {
                dup2(pipefd[i][1], STDOUT_FILENO);
                close(pipefd[i][0]);
                close(pipefd[i][1]);
            }
            
            // if not first duplicate read end of the previous pipe to STDIN
            if (i > 0)
            {
                dup2(pipefd[i - 1][0], STDIN_FILENO);
                close(pipefd[i - 1][0]);
                close(pipefd[i - 1][1]);
            }

            // Handle stdout redirection for the last command in the pipeline
            if (i == numPipes && cmd[startIndex + i].stdout_file != NULL)
            {
                int ofd = open(cmd[startIndex + i].stdout_file, O_CREAT | O_WRONLY | O_TRUNC, 0666);
                if (ofd < 0)
                {
                    perror("open");
                    exit(1);
                }
                dup2(ofd, STDOUT_FILENO);
                close(ofd);
            }

            // Handle stdin redirection for the first command in the pipeline
            if (i == 0 && cmd[startIndex].stdin_file != NULL)
            {
                int ifd = open(cmd[startIndex].stdin_file, O_RDONLY);
                if (ifd < 0)
                {
                    perror("open");
                    exit(1);
                }
                dup2(ifd, STDIN_FILENO);
                close(ifd);
            }

	    // Executes current command in the pipeline
            execute_command(&cmd[startIndex + i]);
            exit(0);
        }
        else // parent process
        {
            // close the read and write of the previous pipes if not first command
            if (i > 0)
            {
                close(pipefd[i - 1][0]);
                close(pipefd[i - 1][1]);
            }
        }
    }

    // Parent process waits for all child processes to finish their jobs.
    for (int i = 0; i <= numPipes; i++)
    {
        wait(NULL);
    }
}

// changes the prompt for the shell
void prompt_handler(char *prompt)
{
    printf("Enter new prompt: ");
    fgets(prompt, PROMPT_SIZE, stdin);
    size_t len = strlen(prompt);
    if (len > 0 && prompt[len - 1] == '\n')
    {
        prompt[len - 1] = '\0';
    }
}

// prints out the pwd.
void pwd_handler()
{
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)))
    {
        printf("%s\n", cwd);
    }
    else
    {
        perror("getcwd");
    }
}

// handles cd paths.
void cd_handler(char *path)
{
    if (chdir(path) == -1)
    {
        perror("chdir");
    }
}

void exit_handler()
{
    exit(0);
}

