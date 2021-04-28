CC=gcc
AC=nasm
AFLAGS= -g -f elf64 -F DWARF
CFLAGS= -Wall -g

all:
	mkdir -p obj
	mkdir -p bin
	$(AC) $(AFLAGS) -o ./obj/main.o ./src/main.asm
	$(AC) $(AFLAGS) -o ./obj/utils.o ./src/utils.asm
	ld -o ./bin/ej_1 ./obj/main.o ./obj/utils.o

test.o:
	mkdir -p obj
	$(CC) $(CFLAGS) -c -o ./obj/test.o ./src/test.c

exec:
	./bin/ej_1

init_dir: 
	mkdir src
	mkdir inc
	mkdir lib
	mkdir sup

.PHONY: clean
clean:
	rm -rf *.o
	rm -r ./bin
	rm -r ./obj

