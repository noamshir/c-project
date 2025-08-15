CC = gcc
CFLAGS = -ansi -pedantic -Wall

assembler: assembler.o pre_assembler.o mcro_table.o string.o files_extractor.o first_pass.o second_pass.o abcd_code.o symbol_table.o utils.o binary_code.o error.o guide.o command.o
	$(CC) $(CFLAGS) -o assembler assembler.o pre_assembler.o mcro_table.o string.o files_extractor.o first_pass.o second_pass.o abcd_code.o symbol_table.o utils.o binary_code.o error.o guide.o command.o

assembler.o: assembler.c Headers/assembler.h Headers/pre_assembler.h Headers/mcro_table.h Headers/string.h Headers/files_extractor.h Headers/first_pass.h Headers/second_pass.h Headers/abcd_code.h Headers/symbol_table.h Headers/utils.h Headers/binary_code.h Headers/error.h Headers/guide.h Headers/command.h
	$(CC) $(CFLAGS) -c assembler.c -o assembler.o

pre_assembler.o: pre_assembler.c Headers/pre_assembler.h Headers/mcro_table.h Headers/string.h Headers/files_extractor.h
	$(CC) $(CFLAGS) -c pre_assembler.c -o pre_assembler.o

mcro_table.o: mcro_table.c Headers/mcro_table.h
	$(CC) $(CFLAGS) -c mcro_table.c -o mcro_table.o

string.o: string.c Headers/string.h
	$(CC) $(CFLAGS) -c string.c -o string.o

files_extractor.o: files_extractor.c Headers/files_extractor.h
	$(CC) $(CFLAGS) -c files_extractor.c -o files_extractor.o

first_pass.o: first_pass.c Headers/first_pass.h Headers/symbol_table.h
	$(CC) $(CFLAGS) -c first_pass.c -o first_pass.o

second_pass.o: second_pass.c Headers/second_pass.h Headers/binary_code.h
	$(CC) $(CFLAGS) -c second_pass.c -o second_pass.o

abcd_code.o: abcd_code.c Headers/abcd_code.h
	$(CC) $(CFLAGS) -c abcd_code.c -o abcd_code.o

symbol_table.o: symbol_table.c Headers/symbol_table.h
	$(CC) $(CFLAGS) -c symbol_table.c -o symbol_table.o

utils.o: utils.c Headers/utils.h
	$(CC) $(CFLAGS) -c utils.c -o utils.o

binary_code.o: binary_code.c Headers/binary_code.h
	$(CC) $(CFLAGS) -c binary_code.c -o binary_code.o

error.o: error.c Headers/error.h
	$(CC) $(CFLAGS) -c error.c -o error.o

guide.o: guide.c Headers/guide.h
	$(CC) $(CFLAGS) -c guide.c -o guide.o

command.o: command.c Headers/command.h
	$(CC) $(CFLAGS) -c command.c -o command.o

clean:
	rm -f *.o
