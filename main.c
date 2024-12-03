#include "executor.h"
#include "parser.h"
#include "proompt.h"
#include "reader.h"
#include <string.h>

int main(void) {
	while (1) {
		printPrompt();
		char * line = readLine();
		unsigned long n;
		char * cmds[] = parseCmds(&n);  // handle semicolons, length in n
		char * exit[] = "exit";
		if (strcmp(cmds[0], exit) == 0) {exit();}
		for (unsigned long i = 0; i < n; i++) { 
			execute(cmds[i]);
		}
		// free(line);  // maybe we need this later
	}
	return 0;
}
