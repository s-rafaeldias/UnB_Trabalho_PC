mode = -std=gnu99
name = main.exe
lib  = -pthread
ciclo = 24

all: compile run run_clean

run: $(name) run_clean
	./$(name) $(ciclo)

compile: main.o cicloTempo.o log.o
	gcc $(mode) main.o cicloTempo.o log.o -o $(name) $(lib)

main.o: main.c
	gcc $(mode) main.c -c $(lib)

cicloTempo.o: cicloTempo.h cicloTempo.c
	gcc $(mode) cicloTempo.c -c

log.o: log.h log.c
	gcc $(mode) log.c -c $(lib)

full_clean:
	rm *.o *.csv *.exe

run_clean:
	rm *.o
