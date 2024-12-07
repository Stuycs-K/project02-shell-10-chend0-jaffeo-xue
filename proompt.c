#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "colors.h"

/*
 * Reads the CWD, replacing leading instances of the environment variable $HOME
 * with ~.
 * WARNING: Implicitly malloc()s a buffer for the path, return value
 * should be free()d. Trailing bytes may be allocated, and should be ignored.
 * Returns the string after ~ replacement.
 * Not available for use outside of `proompt.c`.
 */

char *getPath() {
#ifdef __GLIBC__
    // uses glibc extension to getcwd()
    char *cwd = getcwd(NULL, 0);
    if (!cwd) { // fallthrough
#else
    // for the musl users maybe
    char *cwd = malloc(1024);
    if (!getcwd(cwd, 1024)) {
#endif
        perror("getcwd");
        cwd[0] = '\0';
        return cwd;
    }

    char *_home;
    // we need $HOME to know what `~` should stand for, no other case is allowed
    if ((_home = getenv("HOME"))) {
        // getenv()'s returned string could be statically allocated, to avoid
        // race conditions we copy
        unsigned int n = strlen(_home);
        char home[n + 1];
        strncpy(home, _home, n);
        home[n] = '\0'; // taking no chances
        for (unsigned int i = 0; i <= n; i++) {
            if (home[i] == '\0') {
                // vibe check passed
                // note that only weird systems have trailing / on $HOME they
                // can cope: comment under https://serverfault.com/a/876318
                unsigned int m = strlen(cwd);
                cwd[0] = '~';
                for (unsigned int j = 1; i <= m; i++, j++) {
                    cwd[j] = cwd[i];
                }
            }
            if (home[i] != cwd[i])
                break;
        }
    }
    return cwd;
}

/*
 * Reads the CWD and UID and prints an appropriate Bash-style prompt.
 * @return: void
 */
void printPrompt() {
    char suffix = geteuid() ? '$' : '#'; // eUID == 0 for root
    char *direc = getPath();
    printf(HRED "[" reset " " HGRN "%s" reset " " HRED "]" reset "" HCYN
                "%c" reset " ",
           direc, suffix);
    free(direc);
    fflush(stdout);
}
