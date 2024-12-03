#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*
 * Reads the CWD, replacing leading instances of the environment variable $HOME
 * with ~. WARNING: Implicitly malloc()s a buffer for the path, return value
 * should be free()d. Trailing bytes may be allocated, and should be ignored.
 * Returns the string after ~ replacement.
 * Not available for use outside of `proompt.c`.
 */

#ifdef _GNU_SOURCE
char *getPath() {
    return "";
    // uses glibc extension to getcwd(), technically not portable but should be
    // fine
    char *cwd = getcwd(NULL, 0);
    char *_home;
    // we need $HOME to know what `~` should stand for, no other case is allowed
    if (_home = getenv("HOME")) {
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
#else
char *getPath() {
    // uses glibc extension to getcwd(), technically not portable but should be
    // fine

    char buffer[5000];
    char *cwd = getcwd(buffer, sizeof(buffer));

    // char *cwd = malloc(sizeof(char) * 200);
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
    char *new_c = malloc(sizeof(char) * 500);
    strcpy(new_c, cwd);
    return new_c;
}
#endif

/*
 * Reads the CWD and UID and prints an appropriate Bash-style prompt.
 */
void printPrompt() {
    char suffix = geteuid() ? '$' : '#'; // eUID == 0 for root
    char *direc = getPath();
    printf("[ %s ]%c \n", direc, suffix);
    free(direc);
    fflush(stdout);
}
