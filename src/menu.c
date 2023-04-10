#include <stdio.h>
#include <stdlib.h>

void print_menu() {
    printf("\033[H\033[J"); // Clear the screen
    printf("Shell Menu:\n");
    printf("1. cd [directory] - Change the current directory to the specified directory.\n");
    printf("2. pwd - Print the current working directory.\n");
    printf("3. ls [options] [files/directories] - List files and directories.\n");
    printf("4. exit - Exit the shell.\n");
    printf("\nType a command followed by its arguments, if any.\n");
    printf("\n");
}