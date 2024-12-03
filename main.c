#include "executor.h"
#include "parser.h"
#include "proompt.h"
#include "reader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    while (1) {
        printPrompt();
        char *line = readLine();
        unsigned long n = 0;
        // char **cmds = parseCmds(line, &n); // handle semicolons, length in n
        // char *e = "exit";
        // if (strcmp(cmds[0], e) == 0) {
        //     exit(0);
        // }
        // for (unsigned long i = 0; i < n; i++) {
        //     execute(cmds[i]);
        // }
        execute(line);
        // free(line);  // maybe we need this later
    }
    return 0;
}
