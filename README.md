[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/Tfg6waJb)

# Systems Project2 - MyShell

## DOE Presents: The Practical Operation Orchestration Prompt

Group members: Elias Xu, David Chen, Otzar Jaffe

### Feature Set

Dynamic prompt that

- displays the current working directory, with `~` substituted for `$HOME` when possible
- displays `#` for root sessions, and `$` for other user sessions

The shell can run prompts with pipes in them and commands with redirection.

### Unimplemented

Nothing is unimplemented.

### Bugs

We are not aware of any bugs in our shell.

### Function Reference (alphabetically by file name)

`executor.c`
```c
void print_char_ss(char **args)
char **slice(char **arg_ary, int start, int end, int extra)
void parse_args(char *command, char **arg_ary)
void run(char **args, int input, int output, char *output_file, char *input_file)
void execute_commands(char **args)
void reorganize_pipe(char **args)
void execute(char *command)
```

`parser.c`
```c
char ** parseCmds(char* line, unsigned long *n)
```

`proompt.c`
```c
char *getPath()
void printPrompt()
```

`reader.c`
```c
char *readLine()
```