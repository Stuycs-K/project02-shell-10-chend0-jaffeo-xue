#ifndef EXECUTOR_H
#define EXECUTOR_H
void execute(char *command);
void execute_commands(char *command, char **args);
void run(char *args[16], int input, int output, char *output_file,
         char *input_file);
#endif
