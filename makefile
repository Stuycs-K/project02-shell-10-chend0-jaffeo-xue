.PHONY: compile run clean
compile shell: main.o executor.o parser.o proompt.o reader.o
	@gcc -o shell main.o executor.o parser.o proompt.o reader.o
main.o: main.c
	@gcc -c main.c
executor.o: executor.h executor.c
	@gcc -c executor.c
parser.o: parser.h parser.c
	@gcc -c parser.c
proompt.o: proompt.h proompt.c
	@gcc -c proompt.c
reader.o: reader.h reader.c
	@gcc -c reader.c
run: shell
	@./shell $(ARGS)
clean:
	@-rm -rf shell *.o *.txt
