#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX_SIZE_ARG 20

/*
 * Turns a space-separated command line into an array of words.
 * WARNING: Mutates the argument string.
 * Returns the NULL-terminated array of words.
 * Not available for use outside of `executor.c`.
 * Works the same as the python command.split(" ")[start:end]
 */
void parse_args(char *command, char **arg_ary, int start, int end) {
    int arg_index = 0;
		int total_found = 0;
    while (strstr(command, " ") != NULL) {
        char *c = strsep(&command, " ");
        if (strcmp("", c) != 0 && c[0] != ' ') {
						total_found++;

						// if you have a start value...
						if (start != -1 && start > total_found) {
								continue;
						}
            
						arg_ary[arg_index] = c;
         		arg_index++;

						// if you have an end value...
						if (end != -1 && end > arg_index) {
								break;
						}
        }
    }
    arg_ary[arg_index] = command;
    arg_ary[arg_index + 1] = NULL;
}


/*
 * Parses and executes the command given in `command` using execvp().
 */
void run(char *args[16], int input, int output, char *output_file, char *input_file) {
		int stdout = STDOUT_FILENO;
		int stdin = STDIN_FILENO;
		int backup_stdout = dup(stdout);
		int backup_stdin = dup(stdin);

		// if we are sending something to an input
		if (input > 0) {
				int f_in = 0;
				f_in = open(input_file, O_RDONLY);
				dup2(f_in, stdin); 
		}

		// if we are sending something to an output
		if (output > 0) {
				// if output is 1, then we are overwritting; if it is 2, we are appending
				int fd1 = 0;
				if (output == 1) { fd1 = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644); }
				if (output == 2) { fd1 = open(output_file, O_WRONLY | O_APPEND | O_CREAT, 0644); }
				dup2(fd1, stdout);
		}

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
		dup2(backup_stdout, stdout);
		dup2(backup_stdin, stdin);
}

void execute(char *command) {
		char *args[16];
		char *new_args[16];
		parse_args(command, args, -1, -1);
		run(args, 1, 0, NULL, "executor.c");
		/*
		for(int i = 0; i < 16; i++) {
				if (strcmp(args[i], "|")) {
						parse_args(new_args, 0, i);
						run(new_args, x, x);
						parse_args(new_args, i, 16);
						run(new_args, x, x);
				}
		}*/
} 
