mode = -std=c99
nome = main.exe
lib  = -pthread

run: $(nome)
	./$(nome)

compile: main.o
	gcc $(mode) main.o -o $(nome) $(lib)

main.o: main.c
	gcc $(mode) main.c -c $(lib)

clean:
	rm *.o