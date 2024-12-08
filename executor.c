#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

/*
 * Prints the given array of strings.
 * WARNING: The array must be NULL-terminated.
 * Not available for use outside of `executor.c`.
 * Works the same as the python print(args)
 * @param args: The array of strings to print.
 * @return: void
*/
void print_char_ss(char **args) {
    for (int i = 0;; i++) {
        printf("%d: `%s`\t", i, args[i]);
        if (!args[i])
            break;
    }
    printf("\n");
}

/*
 * Copies a slice of the given array of strings from `start` to `end`
 * (exclusive). The slice be NULL-terminated. WARNING: Returned buffer should be
 * freed after use. Not available for use outside of `executor.c`. Inspired by
 * the Python array slice mechanism.
 * @param arg_ary: The array of strings to slice.
 * @param start: The starting index of the slice.
 * @param end: The ending index of the slice.
 * @param extra: The number of extra spaces to allocate (to prevent buffer
 * overruns).
 * @return: The sliced array of strings.
 */
char **slice(char **arg_ary, int start, int end, int extra) {
    char **sliced_args = malloc(sizeof(char *) * (end - start + extra + 1));
    if (!sliced_args) {
        perror("slice(): malloc");
        exit(errno);
    }
    for (int i = start; i < end; i++) {
        sliced_args[i - start] = arg_ary[i];
    }
    sliced_args[end - start] = NULL;
    return sliced_args;
}

/*
 * Turns a space-separated command line into an array of words.
 * WARNING: Mutates the argument string.
 * WARNING: Requires arg_ary to point to a sufficiently large buffer.
 * Returns the NULL-terminated array of words.
 * Not available for use outside of `executor.c`.
 * @param command: The command line to parse.
 * @param arg_ary: The array of strings to fill with the words.
 * @return: void
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
 * Not available for use outside of `executor.c`.
 * @param args: The array of strings representing the command.
 * @param input: The input mode (0 for no input, 1 for input).
 * @param output: The output mode (0 for no output, 1 for output, 2 for append).
 * @param output_file: The file to write the output to.
 * @param input_file: The file to read the input from.
 * @return: 0 if execution succeeded, errno if failed; exits program on `exit`
 */
int run(char **args, char input, char output, char *input_file,
        char *output_file) {
    int orig_stdout = STDOUT_FILENO, orig_stdin = STDIN_FILENO;
    int backup_stdout = dup(orig_stdout), backup_stdin = dup(orig_stdin);

    if (args[0] == NULL || args[0][0] == '\0')
        return 0;
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
            return errno;
        } else if (p == 0) {
            // input
            if (input > 0) {
                int f_in = open(input_file, O_RDONLY, 0644);
                if (f_in == -1) {
                    perror(input_file);
                    return errno;
                }
                dup2(f_in, orig_stdin);
                close(f_in);
            }

            // output
            if (output > 0) {
                int fd1 = 0;
                int flags = O_WRONLY | O_CREAT;
                if (output == 1)
                    flags |= O_TRUNC;
                else if (output == 2)
                    flags |= O_APPEND;
                else {
                    errno = EINVAL;
                    perror("run(): output");
                    dup2(backup_stdin, orig_stdin);
                    close(backup_stdin);
                    return errno;
                }
                fd1 = open(output_file, flags, 0644);
                if (fd1 == -1) {
                    perror(output_file);
                    dup2(backup_stdin, orig_stdin);
                    close(backup_stdin);
                    return errno;
                }
                dup2(fd1, orig_stdout);
                close(fd1);
            }
            execvp(args[0], args);
            perror(args[0]);
        } else {
            int status;
            int exit_pid = wait(&status);
        }
    }
    dup2(backup_stdout, orig_stdout);
    dup2(backup_stdin, orig_stdin);
    close(backup_stdin);
    close(backup_stdout);
    return 0;
}

/*
 * Executes the given command.
 * Not available for use outside of `executor.c`.
 * @param args: The command to execute, after parsing by parse_args().
 * @return: 0 if successful, `errno` from run() if failed
 */
int execute_commands(char **args) {
    char **new_args = NULL;
    char *input_file = NULL, *output_file = NULL;

    // output_mode could be 1 or 2 for > or >> respectively
    int input_mode = 0, output_mode = 0;
    int index_last_command = 0;

    for (int i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], "<") == 0 || strcmp(args[i], ">") == 0 ||
            strcmp(args[i], ">>") == 0) {
            if (strcmp(args[i], "<") == 0) {
                i += 1;
                input_file = args[i];
                input_mode = 1;
                if (new_args == NULL) {
                    new_args = slice(args, index_last_command, i - 1, 0);
                }
            } else if (strcmp(args[i], ">") == 0) {
                i += 1;
                output_file = args[i];
                output_mode = 1;
                if (new_args == NULL) {
                    new_args = slice(args, index_last_command, i - 1, 0);
                }
            } else /* if (strcmp(args[i], ">>") == 0) */ {
                i += 1;
                output_file = args[i];
                output_mode = 2;
                if (new_args == NULL) {
                    new_args = slice(args, index_last_command, i - 1, 0);
                }
            }
            index_last_command = i + 1;
        }
    }

    if (new_args != NULL) {
        int ret =
            run(new_args, input_mode, output_mode, input_file, output_file);
        free(new_args);
        return ret;
    } else {
        return run(args, input_mode, output_mode, input_file, output_file);
    }
}

/*
 * Reorganizes the command to fake a single pipe and executes it.
 * Not available for use outside of `executor.c`.
 * @param args: The command to process and execute.
 * @return: void
 */
void reorganize_pipe(char **args) {
    int pipe = -1, size = 0;
    for (int i = 0; args[i] != NULL; i++) {
        if (pipe < 0 && strcmp(args[i], "|") == 0) {
            pipe = i;
        }
        size = i + 1;
    }

    if (pipe > 0) {
        char **args1, **args2;
        char *temporary_file = NULL;

        for (int i = 0; i < pipe - 1; i++) {
            if (strstr(args[i], ">") !=
                NULL /* || strstr(args[i], ">>") != NULL */) {
                if (temporary_file)
                    free(temporary_file);
                size_t len = strlen(args[i + 1]) + 1;
                temporary_file = malloc(sizeof(char) * len);
                if (!temporary_file) {
                    perror("reorganize_pipe: malloc");
                    exit(errno);
                }
                strncpy(temporary_file, args[i + 1], len);
                temporary_file[len - 1] = '\0'; // safety first!
            }
        }
        if (temporary_file == NULL) {
            temporary_file = "temp.txt";
        }

        args1 = slice(args, 0, pipe, 3);
        args1[pipe] = ">";
        args1[pipe + 1] = temporary_file;
        args1[pipe + 2] = NULL;

        args2 = slice(args, pipe + 1, size, 3);
        int len_args2 = size - pipe - 1;
        args2[len_args2] = "<";
        args2[len_args2 + 1] = temporary_file;
        args2[len_args2 + 2] = NULL;
        if (!execute_commands(args1))
            execute_commands(args2);

        if (strcmp(temporary_file, "temp.txt") == 0) {
            if (remove(temporary_file) == -1 && errno != ENOENT)
                perror("reorganize_pipe(): temp.txt");
        }
        free(args1);
        free(args2);
        // [unfree()able] string constant will have same address everywhere
        if (temporary_file != "temp.txt")
            free(temporary_file);
    } else {
        execute_commands(args);
    }
}

/*
 * Executes the given command.
 * @param command: The command to execute.
 * @return: void
 */
void execute(char *command) {
    // heuristic buffer sizing method
    char *args[strlen(command) / 2 + 2];
    parse_args(command, args);
    reorganize_pipe(args);
}
