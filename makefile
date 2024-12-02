.PHONY: compile run clean
compile shell: main.o
	@gcc -o shell main.o
main.o: main.c executor.o parser.o prompt.o reader.o
	@gcc -c main.c
executor.o: executor.h executor.c
	@gcc -c executor.c
parser.o: parser.h parser.c
	@gcc -c parser.c
prompt.o: prompt.h prompt.c
	@gcc -c prompt.c
reader.o: reader.h reader.c
	@gcc -c reader.c
run: shell
	@./shell $(ARGS)
clean:
	@-rm -rf shell *.o *.txt
