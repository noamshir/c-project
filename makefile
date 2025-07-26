main: main.c pre_assembler.c table.c string.c files_extractor.c
	gcc -ansi -pedantic -Wall main.c pre_assembler.c table.c string.c files_extractor.c -o main

clean:
	rm -f main


