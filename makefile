.PHONY: compile run clean
compile shell: main.o
	@gcc -o shell main.o
main.o: main.c
	@gcc -c main.c
run: shell
	@./shell $(ARGS)
clean:
	@-rm -rf shell *.o *.txt
