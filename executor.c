#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>


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
        if (strcmp("", c) != 0 && c[0] != ' ') {
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
    // TODO MAKE DYNAMIC
    char *args[16];
    parse_args(command, args);
    if (args[0] == NULL || args[0][0] == '\0')
        return;
    else if (!strcmp(args[0], "cd")) {
        char *path = args[1]; // in worst case is NULL
        if (!path)
            path = getenv("HOME");
        if (path) { // could be still NULL from being HOMEless
            if (chdir(path))
		    perror(path);
	}
    } else if (!strcmp(args[0], "exit")) {
        exit(0);
    } else {
        pid_t p;
        p = fork();
        if (p < 0) {
            perror("fork");
            return;
        } else if (p == 0) {
            execvp(args[0], args);
            perror(args[0]);
            exit(errno);
        } else {
            int status;
            int exit_pid = wait(&status);
        }
    }
}
