.PHONY: clean

BINPATH = ./bin/
DOCPATH = ./doc/
INCPATH = ./inc/
LIBPATH = ./lib/
SRCPATH = ./src/
SUPPATH = ./sup/

AC = nasm
CC = gcc
AFLAGS = -f elf32
CFLAGS = -c -Wall
IFLAGS = -mgeneral-regs-only #Flags para compilar los handler de interrupciones en C
LDFLAGS = -z max-page-size=0x01000 --oformat=binary -m elf_i386

LDSCRIPT = linker.ld
ODFLAGS = -CprsSx --prefix-addresses
#en objs guardo todos mis objetos elf
OBJS = $(BINPATH)main.o

SRC_NAME = ejecutable

#all: $(BINPATH)$(SRC_NAME).o
all: $(OBJS)
	@echo Generando $(SRC_NAME)
	$(CC) $(OBJS) -o $(SRC_NAME)
	@echo Ejecutando $(SRC_NAME)
	./ejecutable

$(BINPATH)main.o: $(SRCPATH)main.c
	@echo Generando $@
	mkdir -p bin
	$(CC) $(CFLAGS) $< -o $@


clean:
	@echo limpiando carpeta..
	rm $(BINPATH)*.bin $(BINPATH)*.o $(BINPATH)*.elf $(BINPATH)*.lst *.map $(SRC_NAME)