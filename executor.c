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
    printf("command %s\n", command);
    int arg_index = 0;
    while (strstr(command, " ") != NULL) {
        char *c = strsep(&command, " ");
        if (strcmp("", c) != 0 || strcmp(" ", c) != 0) {
            arg_ary[arg_index] = strsep(&command, " ");
            arg_index++;
        }
    }
    printf("hello27\n");
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
    char *args[16];
    if (p == 0) {
        if (command[0] != 'c' && command[1] != 'd') {
            parse_args(command, args);
            execvp(args[0], args);
            exit(0);
        }
        exit(0);
    } else {
        int status;
        int exit_pid = wait(&status);
        if (command[0] == 'c' && command[1] == 'd') {
            parse_args(command, args);
            char *cwd = malloc(5056);
            if (!getcwd(cwd, 5056)) {
                perror("getcwd\n");
                cwd[0] = '\0';
                return;
            }
            printf("sigma56\n");
            printf("cwd: %s\n", cwd);
            char *diff = "/";
            char buff[2000];
            strcpy(buff, cwd);
            strcat(buff, diff);
            printf(buff);
            // strcat(s, args[1])

            // chdir(strcat(strcat(cwd, "/"), args[1]));
        }
    }
}
