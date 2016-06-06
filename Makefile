CC = gcc
mode = -std=gnu99
name = main.exe
lib  = -pthread
ciclo = 24

all: compile run run_clean

run: $(name) run_clean
	./$(name) $(ciclo)

compile: main.o cicloTempo.o log.o
	$(CC) $(mode) main.o cicloTempo.o log.o -o $(name) $(lib)

main.o: main.c
	$(CC) $(mode) main.c -c $(lib)

cicloTempo.o: cicloTempo.h cicloTempo.c
	$(CC) $(mode) cicloTempo.c -c

log.o: log.h log.c
	$(CC) $(mode) log.c -c $(lib)

full_clean: run_clean
	rm -f *.exe

run_clean:
	rm -f *.o *.csv
