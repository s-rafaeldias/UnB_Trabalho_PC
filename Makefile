mode = -std=c99
nome = main.exe
lib  = -pthread

all: program

teste: main.o
	gcc $(mode) main.o -o $(nome) $(lib)

main.o: main.c
	gcc $(mode) main.c -c $(lib)

clean:
	rm *.o
