main: main.o table.o
	gcc -ansi -pedantic -Wall main.o table.o -o main

main.o: main.c Headers/table.h
	gcc -ansi -pedantic -Wall -c main.c

table.o: table.c Headers/table.h
	gcc -ansi -pedantic -Wall -c table.c

clean:
	rm -f *.o main

.PHONY: clean
