main: main.c pre_assembler.c table.c string.c files_extractor.c first_pass.c symbol_table.c utils.c
	gcc -ansi -pedantic -Wall main.c pre_assembler.c table.c string.c files_extractor.c first_pass.c symbol_table.c utils.c -o main

clean:
	rm -f main


