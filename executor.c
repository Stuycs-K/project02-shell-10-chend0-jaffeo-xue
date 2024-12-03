#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define NUMBER_OF_ARGS_MAX 30
#define MAX_SIZE_ARG 20

/*
 * Turns a space-separated command line into an array of words.
 * WARNING: Mutates the argument string.
 * Returns the NULL-terminated array of words.
 * Not available for use outside of `executor.c`.
 */
char **parseCmd(char *c) {
    char *arg_ary[NUMBER_OF_ARGS_MAX];
    int arg_index = 0;
    char *command = strdup(c);
    while (strstr(command, " ") != NULL) {
        arg_ary[arg_index] = strsep(&command, " ");
        arg_index++;
    }
    arg_ary[arg_index] = command;
    arg_ary[arg_index + 1] = NULL;
    char **memorized_elements = malloc((arg_index + 1) * sizeof(char *));
    for (int i = 0; i < (arg_index + 1); i++) {
        memorized_elements[i] = malloc(sizeof(char) * MAX_SIZE_ARG);
        strcpy(memorized_elements[i], arg_ary[i]);
    }
    return memorized_elements;
}

/*
 * Parses and executes the command given in `command` using execvp().
 */
void execute(char *command) {}
