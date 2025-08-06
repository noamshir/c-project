main: assembler.c pre_assembler.c mcro_table.c string.c files_extractor.c first_pass.c second_pass.c abcd_code.c  symbol_table.c utils.c binary_code.c error.c guide.c command.c
	gcc -ansi -pedantic -Wall assembler.c pre_assembler.c mcro_table.c string.c files_extractor.c first_pass.c second_pass.c abcd_code.c symbol_table.c utils.c binary_code.c error.c guide.c command.c -o assembler

clean:
	rm -f assembler


