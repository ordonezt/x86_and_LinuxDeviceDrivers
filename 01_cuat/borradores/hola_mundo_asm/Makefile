CC=gcc
CFLAGS= -Wall

all:
	mkdir bin
	mkdir obj
	nasm -g -f elf64 -F DWARF -o ./obj/hola_mundo.o ./src/hola_mundo.asm
	ld ./obj/hola_mundo.o -o ./bin/hola_mundo -g

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

