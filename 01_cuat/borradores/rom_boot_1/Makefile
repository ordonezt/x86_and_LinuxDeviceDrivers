out=./bin/mibios.bin
NASM=nasm
NFLAGS=-fbin -l$(LIST) -Wall # -l hace que se genere un archivo listing con lo que hizo el compilador
OBJS=
BIN=./bin/mibios.bin
SOURCES=./src/rom_boot.asm
LIST=./lst/rom_boot.lst

$(out): $(SOURCES) Makefile
	mkdir bin
	mkdir lst
	$(NASM) $(NFLAGS) $(SOURCES) -o$@ 
bochs:
	bochs -q
clean:
	rm -r bin
	rm -r lst
	rm -fr $(out) *.lst