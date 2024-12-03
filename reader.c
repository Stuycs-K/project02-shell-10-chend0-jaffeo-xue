#include <stdio.h>
#include <stdlib.h>
/*
 * Reads a newline-delimited line of input from stdin. May exit on EOF if no
 * buffered input is found. WARNING: malloc()s a new block of data potentially
 * larger than user input. Do not forget to free(). Returns the read line of
 * input.
 */

// kludge for now
#define SIZE_STATIC 256
char *readLine() {
    char *ret = malloc(SIZE_STATIC * (sizeof(char)));
    fgets(ret, SIZE_STATIC, stdin);
    return ret;
}
