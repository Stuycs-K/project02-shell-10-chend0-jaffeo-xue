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
            arg_ary[arg_index] = strsep(&command, " ");
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
    pid_t p;
    p = fork();
    if (p < 0) {
        perror("fork fail\n");
        return;
    }
    if (p == 0) {
        char *args[16];
        parse_args(command, args);
        execvp(args[0], args);
    } else {
        int status;
        int exit_pid = wait(&status);
    }
}
