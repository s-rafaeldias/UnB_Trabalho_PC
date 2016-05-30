mode = -std=gnu99
name = main.exe
lib  = -pthread
ciclo = 24

run: $(name)
	./$(name) $(ciclo)

compile: main.o cicloTempo.o log.o
	gcc $(mode) main.o cicloTempo.o log.o -o $(name) $(lib)

main.o: main.c
	gcc $(mode) main.c -c $(lib)

cicloTempo.o: cicloTempo.h cicloTempo.c
	gcc $(mode) cicloTempo.c -c

log.o: log.h log.c
	gcc $(mode) log.c -c $(lib)

clean:
	rm *.o *.csv
