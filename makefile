main: main.c table.c string.c
	gcc -ansi -pedantic -Wall main.c table.c string.c -o main

clean:
	rm -f main


