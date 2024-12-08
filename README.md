[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/Tfg6waJb)

# Systems Project2 - MyShell

## The DOE (David, Otzar, Elias) Presents: The Practical Operation Orchestration Prompt (POOP)

Group members: Elias Xu, David Chen, Otzar Jaffe

### Feature Set

- Execution of inputted commands, with **all** spaces treated as argument separators.
  - Implements `cd` and `exit`, along with EOF termination.
- Dynamic prompt that
  - displays the current working directory, with `~` substituted for `$HOME` when possible
  - displays `#` for root sessions, and `$` for other user sessions
- File input/output redirection
  - A single (1) fake pipe using `temp.txt` as a buffer.
- Semi-colon separation of commands
- Stripping handling of multiple space characters.

### Unimplemented

Additional extensions we were considering (but did not implement) include:
- True piping with `pipe()`
  - Parent-process management of file handles and simultaneous execution using the following linked-list structure, which would accomodate multiple outputs (which is possible in Bash):
```c
struct process {
    char **command;
    int input_fd;
    int *output_fds;
    size_t output_n;
    struct process *next;
}
```
- Parsing of escape characters using `\` (`\n`, `\t`, etc.)
- Quoting strings to allow for arguments with spaces
- Environment variable substitution and modification.
  - `cd -` implementation using `$OLDPWD` (like bash)

### Bugs

We are not aware of any bugs in our shell, save for a seemingly negligible Valgrind memcheck notice when reading from `getcwd()`.

### Function Reference

`executor.c`
```c
/*
 * Prints the given array of strings.
 * WARNING: The array must be NULL-terminated.
 * Not available for use outside of `executor.c`.
 * Works the same as the python print(args)
 * @param args: The array of strings to print.
 * @return: void
*/
void print_char_ss(char **args);

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
char **slice(char **arg_ary, int start, int end, int extra);

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
void parse_args(char *command, char **arg_ary);

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
int run(char **args, char input, char output, char *input_file, char *output_file);

/*
 * Executes the given command.
 * Not available for use outside of `executor.c`.
 * @param args: The command to execute, after parsing by parse_args().
 * @return: 0 if successful, `errno` from run() if failed
 */
int execute_commands(char **args);

/*
 * Reorganizes the command to fake a single pipe and executes it.
 * Not available for use outside of `executor.c`.
 * @param args: The command to process and execute.
 * @return: void
 */
void reorganize_pipe(char **args);

/*
 * Executes the given command.
 * @param command: The command to execute.
 * @return: void
 */
void execute(char *command);
```

`parser.c`
```c
/*
 * Separates a semi-colon separated line of user input into an array of strings.
 * WARNING: Mutates the inputted line.
 * WARNING: Returns a malloc()ed array, return value should be free()d.
 * @param line: The line to parse.
 * @param n: The number of commands in the line.
 * Returns the array of strings and writes the length of said array to `n`.
 */
char **parseCmds(char* line, unsigned long *n);
```

`proompt.c`
```c
/*
 * Reads the CWD, replacing leading instances of the environment variable $HOME
 * with ~.
 * WARNING: Implicitly malloc()s a buffer for the path, return value
 * should be free()d. Trailing bytes may be allocated, and should be ignored.
 * Returns the string after ~ replacement.
 * Not available for use outside of `proompt.c`.
 */
char *getPath();

/*
 * Reads the CWD and UID and prints an appropriate Bash-style prompt.
 * @return: void
 */
void printPrompt();
```

`reader.c`
```c
/*
 * Reads a newline-delimited line of input from stdin. May exit on EOF if no
 * buffered input is found. WARNING: malloc()s a new block of data potentially
 * larger than user input. Do not forget to free().
 * @return: ret, the read line of input.
 */
char *readLine();
```
