#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
 * Reads a newline-delimited line of input from stdin. May exit on EOF if no
 * buffered input is found. WARNING: malloc()s a new block of data potentially
 * larger than user input. Do not forget to free(). Returns the read line of
 * input.
 */
char *readLine() {
    unsigned long i = 0, size = 16;
    char *ret = malloc(size * (sizeof(char)));
    if (!ret) {
        perror("malloc");
        exit(errno);
    }
    int buf;
    char endLoop = 0;
    while ((buf = fgetc(stdin)) != EOF) {
        switch (buf) {
        // not necessary since human input is line-buffered but hey nice to have
        case 0177: // DEL
            ret[i] = '\0';
            i--;
            break;
        case '\r':
        case '\n':
            endLoop = 1;
            buf = '\0';
        // fallthrough
        default:
            if (i >= size) {
                size *= 2;
                if (!(ret = realloc(ret, size * sizeof(char)))) {
                    perror("calloc");
                    exit(errno);
                }
            }
            ret[i] = buf;
            i++;
        }
        if (endLoop)
            break;
    }
    if (buf == EOF)
        if (!i) // no buffered input
            exit(0);
    return ret;
}
