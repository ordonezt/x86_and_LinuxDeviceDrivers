CC=gcc
CFLAGS= -Wall
OBJECTS= main.o

hola_mundo:
	mkdir bin
	mkdir obj
	$(CC) $(CFLAGS) ./src/main.c -o ./bin/hola_mundo

exec:
	./bin/hola_mundo

init_dir: 
	mkdir src
	mkdir inc
	mkdir lib
	mkdir sup

.PHONY: clean
clean:
	rm -R ./bin
	rm -R ./obj