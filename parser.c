#include <stdlib.h>
#include <string.h>
/*
 * Separates a semi-colon separated line of user input into an array of strings.
 * WARNING: Mutates the inputted line.
 * Returns the array of strings and writes the length of said array to `n`.
 */
char **parseCmds(char *line, unsigned long *n) {
    char **arg_ary = malloc(sizeof(char *) * (strlen(line) / 2));
    for (int i = 0; i < (strlen(line) / 2); i++) {
        arg_ary[i] = malloc(sizeof(char) * (strlen(line) + 1));
    }
    int arg_index = 0;
    while (strstr(line, ";") != NULL) {
        char *c = strsep(&line, ";");
        if (strcmp("", c) != 0 || strcmp(" ", c) != 0) {
            strcpy(arg_ary[arg_index], c);
            arg_index++;
        }
    }
    arg_ary[arg_index] = line;
    (*n) = arg_index + 1;
    return arg_ary;
}
