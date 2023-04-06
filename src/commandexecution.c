#include "../include/commandexecution.h"
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

void execute_command(Command *command)
{
    pid_t pid;
    int ofd = -1; // stdout redirection
    int ifd = -1; // stdin redirection

    // Handle redirections
    if (command->stdout_file != NULL)
    {
        ofd = open(command->stdout_file, O_CREAT | O_WRONLY | O_TRUNC, 0666);
        if (ofd < 0)
        {
            perror("open");
            exit(1);
        }
    }
    if (command->stdin_file != NULL)
    {
        ifd = open(command->stdin_file, O_RDONLY);
        if (ifd < 0)
        {
            perror("open");
            exit(1);
        }
    }

    if ((pid = fork()) < 0)
    {
        perror("fork\n");
        exit(1);
    }

    if (pid == 0)
    { // child
        if (ofd >= 0)
        {
            dup2(ofd, 1);
            close(ofd);
        }
        else if (ifd >= 0)
        {
            dup2(ifd, 0);
            close(ifd);
        }

        if (execvp(command->argv[0], command->argv) < 0)
        {
            printf("Command '%s' failed\n", command->argv[0]);
            exit(1);
        }
    }
    else
    { // parent
        if (ofd >= 0)
        {
            close(ofd);
            command->stdout_file = NULL;
        }
        else if (ifd >= 0)
        {
            close(ifd);
            command->stdin_file = NULL;
        }

        if (strcmp(command->sep, seqSep) == 0 || strcmp(command->sep, pipeSep) == 0)
        {
            wait(NULL);
        }
        else if (strcmp(command->sep, conSep) == 0)
        {
            printf("\n[%d] Background process \n\n", pid);
        }
        else
        {
            wait(NULL); // Add this line to wait for the child process when the separator is not '&'
        }
    }
    return;
}

void execute_pipe(Command *cmd, int startIndex, int numPipes)
{
    int p[2];
    pid_t pid;

    if (numPipes == 0)
    {
        execute_command(&cmd[startIndex]);
        return;
    }

    if (pipe(p) < 0)
    {
        perror("Error in piping\n");
        return;
    }

    if ((pid = fork()) < 0)
    {
        perror("Error in forking\n");
        return;
    }

    if (pid == 0)
    {
        dup2(p[1], STDOUT_FILENO);
        close(p[1]);
        close(p[0]);

        if (cmd[startIndex].stdout_file != NULL)
        {
            int fd = open(cmd[startIndex].stdout_file, O_WRONLY | O_CREAT, 0766);
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }

        execute_command(&cmd[startIndex]);
        perror("Error in execvp");
        return;
    }
    else
    {
        wait(NULL);
        dup2(p[0], STDIN_FILENO);
        close(p[0]);
        close(p[1]);
        execute_pipe(cmd, startIndex + 1, numPipes - 1);
    }
}

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
