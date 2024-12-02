#include <stdio.h>

int main(void) {
	while (1) {
		printPrompt();
		char * line = readLine();
		unsigned long n;
		char * cmds[] = parseCmds(&n);  // handle semicolons, length in n
		for (unsigned long i = 0; i < n; i++) {
			execute(cmds[i]);
		}
	}
	return 0;
}
