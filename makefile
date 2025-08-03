main: main.c pre_assembler.c mcro_table.c string.c files_extractor.c first_pass.c symbol_table.c utils.c binary_code.c error.c guide.c command.c
	gcc -ansi -pedantic -Wall main.c pre_assembler.c mcro_table.c string.c files_extractor.c first_pass.c symbol_table.c utils.c binary_code.c error.c guide.c command.c -o main

clean:
	rm -f main


