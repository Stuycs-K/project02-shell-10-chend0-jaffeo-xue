#include "executor.h"
#include "parser.h"
#include "proompt.h"
#include "reader.h"

int main(void) {
	while (1) {
		printPrompt();
		char * line = readLine();
		unsigned long n;
		char ** cmds = parseCmds(line, &n);  // handle semicolons, length in n
		for (unsigned long i = 0; i < n; i++) {
			execute(cmds[i]);
		}
		// free(line);  // maybe we need this later
	}
	return 0;
}
