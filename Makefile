mode = -std=gnu99
name = main.exe
lib  = -pthread
ciclo = 24

run: $(name)
	./$(name) $(ciclo)

compile: main.o cicloTempo.o
	gcc $(mode) main.o cicloTempo.o -o $(name) $(lib)

main.o: main.c
	gcc $(mode) main.c -c $(lib)

cicloTempo.o: cicloTempo.h cicloTempo.c
	gcc $(mode) cicloTempo.c -c

clean:
	rm *.o
