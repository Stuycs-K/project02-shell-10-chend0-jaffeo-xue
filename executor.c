#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_SIZE_ARG 20

/*
 * Turns a space-separated command line into an array of words.
 * WARNING: Mutates the argument string.
 * Returns the NULL-terminated array of words.
 * Not available for use outside of `executor.c`.
 */
void parse_args(char *command, char **arg_ary, int start, int end) {
    int arg_index = 0;
		int total_found = 0;
    while (strstr(command, " ") != NULL) {
        char *c = strsep(&command, " ");
        if (strcmp("", c) != 0 && c[0] != ' ') {
						total_found++;

						// if you have a start value...
						if (start != NULL && start > total_found) {
								continue;
						}
            
						arg_ary[arg_index] = c;
         		arg_index++;

						// if you have an end value...
						if (end != NULL && end > arg_index) {
								break;
						}
        }
    }
    arg_ary[arg_index] = command;
    arg_ary[arg_index + 1] = NULL;
}

void execute(char *command) {
		char *args[16];
		char *new_args[16];
		parse_args(command, args, NULL, NULL);

		for(int i = 0; i < 16; i++) {
				if (strcmp(args[i], "<")) {
						parse_args(new_args, 0, i);
						run(new_args, x, x);
						parse_args(new_args, i, 16);
						run(new_args, x, x);
				}
		}
} 

/*
 * Parses and executes the command given in `command` using execvp().
 */
void run(char *args[16], int input, int output) {
    // TODO MAKE DYNAMIC
    if (args[0] == NULL || args[0][0] == '\0')
        return;
    else if (!strcmp(args[0], "cd")) {
        char *path = args[1]; // in worst case is NULL
        if (!path)
            path = getenv("HOME");
        if (path) // could be still NULL from being HOMEless
            chdir(path);
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
