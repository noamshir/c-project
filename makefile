main: main.c pre_assembler.c table.c string.c
	gcc -ansi -pedantic -Wall main.c pre_assembler.c table.c string.c -o main

clean:
	rm -f main


