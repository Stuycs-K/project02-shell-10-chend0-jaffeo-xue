#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_SIZE_ARG 20

/*
 * Turns a space-separated command line into an array of words.
 * WARNING: Mutates the argument string.
 * Returns the NULL-terminated array of words.
 * Not available for use outside of `executor.c`.
 */

void parse_args(char *command, char **arg_ary) {
    int arg_index = 0;
    while (strstr(command, " ") != NULL) {
        char *c = strsep(&command, " ");
        if (strcmp("", c) != 0 || strcmp(" ", c) != 0) {
            arg_ary[arg_index] = c;
            arg_index++;
        }
    }
    arg_ary[arg_index] = command;
    arg_ary[arg_index + 1] = NULL;
}

/*
 * Parses and executes the command given in `command` using execvp().
 */
void execute(char *command) {
    char *args[16];
    parse_args(command, args);
    pid_t p;
    p = fork();
    if (p < 0) {
        perror("fork fail\n");
        return;
    }
    if (p == 0) {
        if (strcmp(args[0], "cd") != 0) {
            execvp(args[0], args);
            exit(0);
        }
        exit(0);
    } else {
        int status;
        int exit_pid = wait(&status);
        if (strcmp(args[0], "cd") == 0) {
            char *cwd = malloc(5056);
            if (!getcwd(cwd, 5056)) {
                perror("getcwd\n");
                cwd[0] = '\0';
                return;
            }
            char *diff = "/";
            char buff[2000];
            strcpy(buff, cwd);
            strcat(buff, diff);
            if (args[1] != NULL) {
                strcat(buff, args[1]);
            }
            chdir(buff);
        }
    }
}
