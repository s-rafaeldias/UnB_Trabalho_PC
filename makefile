mode = -std=c99
nome = main.exe

all: program

teste: main.o
	gcc $(mode) main.o -o $(nome) -pthread

main.o: main.c
	gcc $(mode) main.c -c -pthread

clean:
	rm *.o
