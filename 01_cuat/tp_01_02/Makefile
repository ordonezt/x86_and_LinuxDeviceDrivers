out=./bin/mibios.bin
NASM=nasm
NFLAGS=-o $(out) -fbin -l $(LIST) -Wall
OBJS=
BIN= ./bin/mibios.bin
SOURCES= ./src/memcopy.asm
LIST=./lst/memcopy.lst

$(out): $(SOURCES) Makefile
	mkdir lst
	mkdir bin
	$(NASM) $(NFLAGS) $(SOURCES)
bochs:
	bochs -q
clean:
	rm -r lst
	rm -r bin
	rm -fr $(out) *.lst