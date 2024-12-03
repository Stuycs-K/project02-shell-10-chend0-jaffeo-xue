#include <string.h>
#include <stdlib.h>
/*
 * Separates a semi-colon separated line of user input into an array of strings.
 * WARNING: Mutates the inputted line.
 * Returns the array of strings and writes the length of said array to `n`.
 */
char ** parseCmds(char* line, unsigned long *n) {
    unsigned long size = 1, i = 0;
    char** cmds = malloc(sizeof(char*));
    char* cur = line;
    do {
        if (i > size) {
            // we're ArrayListing this today
            size *= 2;
            cmds = realloc(cmds, size * (sizeof(char*)));
        }
        cmds[i] = cur;
        strsep(&cur, ";");
        i++;
    } while ( cur );
    *n = i;
    return cmds;
}
