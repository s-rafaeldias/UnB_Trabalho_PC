mode = -std=c99
name = main.exe
lib  = -pthread
ciclo = 24


run: $(name)
	./$(name) $(ciclo)

compile: main.o
	gcc $(mode) main.o -o $(name) $(lib)

main.o: main.c
	gcc $(mode) main.c -c $(lib)

clean:
	rm *.o