/*
 * Reads the CWD, replacing leading instances of the environment variable $HOME with ~.
 * Trailing bytes may be allocated, and should be ignored.
 * Returns the string after ~ replacement.
 * Not available for use outside of `proompt.c`.
 */
char * getPath() {
	return "";
}


/*
 * Reads the CWD and UID and prints an appropriate Bash-style prompt.
 */
void printPrompt() {

}
