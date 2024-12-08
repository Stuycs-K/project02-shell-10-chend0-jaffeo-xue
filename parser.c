#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
 * Separates a semi-colon separated line of user input into an array of strings.
 * WARNING: Mutates the inputted line.
 * WARNING: Returns a malloc()ed array, return value should be free()d.
 * @param line: The line to parse.
 * @param n: The number of commands in the line.
 * Returns the array of strings and writes the length of said array to `n`.
 */
char **parseCmds(char *line, unsigned long *n) {
    unsigned long size = 1, i = 0;
    char **cmds = malloc(sizeof(char *));
    if (!cmds) {
        perror("parseCmds(): malloc");
        exit(errno);
    }
    char *cur = line;
    do {
        if (i >= size) {
            // we're ArrayListing this today
            size *= 2;
            if (!(cmds = realloc(cmds, size * (sizeof(char *))))) {
                perror("parseCmds(): realloc");
                exit(errno);
            }
        }
        cmds[i] = cur;
        strsep(&cur, ";");
        i++;
    } while (cur);
    *n = i;
    return cmds;
}
