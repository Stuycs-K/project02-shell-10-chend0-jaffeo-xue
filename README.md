[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/Tfg6waJb)

# Systems Project2 - MyShell

## DOE Presents: The Practical Operation Orchestration Prompt

Group members: Elias Xu, David Chen, Otzar Jaffe

### Feature Set

Dynamic prompt that

- displays the current working directory, with `~` substituted for `$HOME` when possible
- displays `#` for root sessions, and `$` for other user sessions

### Unimplemented

everything, for now...

### Bugs

### Function Reference

`executor.h`:

```c
/*
 * Turns a space-separated command line into an array of words.
 * WARNING: Mutates the argument string.
 * Returns the NULL-terminated array of words.
 * Not available for use outside of `executor.c`.
 */
char** parseCmd(char * command);

/*
 * Parses and executes the command given in `command` using execvp().
 */
void execute(char *command);
```

`parser.h`:

```c
/*
 * Separates a semi-colon separated line of user input into an array of strings.
 * WARNING: Mutates the inputted line.
 * Returns the array of strings and writes the length of said array to `n`.
 */
char ** parseCmds(char* line, unsigned long *n);
```

`proompt.h` (blame Elias for the name):

```c
/*
 * Reads the CWD, replacing leading instances of the environment variable $HOME with ~.
 * Trailing bytes may be allocated, and should be ignored.
 * WARNING: Implicitly malloc()s a buffer for the path, return value should be free()d.
 * Returns the string after ~ replacement.
 * Not available for use outside of `proompt.c`.
 */
char * getPath();

/*
 * Reads the CWD and UID and prints an appropriate Bash-style prompt.
 */
void printPrompt();
```

`reader.h`:

```c
/*
 * Reads a newline-delimited line of input from stdin. May exit on EOF if no buffered input is found.
 * WARNING: malloc()s a new block of data potentially larger than user input. Do not forget to free().
 * Returns the read line of input.
 */
char * readLine();
```
