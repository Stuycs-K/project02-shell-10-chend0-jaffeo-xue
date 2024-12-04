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
        char **cmds = parseCmds(line, &n); // handle semicolons, length in n
        for (unsigned long i = 0; i < n; i++) {
            execute(cmds[i]);
        }
        free(line);
        free(cmds);
    }
    return 0;
}
