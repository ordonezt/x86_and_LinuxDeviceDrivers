.PHONY: clean

BINPATH = ../../bin/
DOCPATH = ../../doc/
INCPATH = ../../inc/
LIBPATH = ../../lib/
SRCPATH = ../../src/
SUPPATH = ../../sup/

AC = nasm
CC = gcc
AFLAGS = -f elf32
CFLAGS = -c -Wall
IFLAGS = -mgeneral-regs-only #Flags para compilar los handler de interrupciones en C
LDFLAGS = -z max-page-size=0x01000 --oformat=binary -m elf_i386

LDSCRIPT = linker.ld
ODFLAGS = -CprsSx --prefix-addresses
#en objs guardo todos mis objetos elf
OBJS = 	$(BINPATH)servidor.o 	$(BINPATH)bitarray.o 	$(BINPATH)config.o 		$(BINPATH)error.o	\
		$(BINPATH)log.o 		$(BINPATH)memory.o 		$(BINPATH)process.o 	$(BINPATH)string.o	\
		$(BINPATH)temporal.o 	$(BINPATH)txt.o 		$(BINPATH)dictionary.o 	$(BINPATH)list.o	\
		$(BINPATH)queue.o

SRC_NAME = servidor

#all: $(BINPATH)$(SRC_NAME).o
all: $(OBJS)
	@echo Generando $(SRC_NAME)
	$(CC) $(OBJS) -o $(SRC_NAME)
	@echo Ejecutando $(SRC_NAME)
	./$(SRC_NAME)

$(BINPATH)servidor.o: $(SRCPATH)servidor/servidor.c
	@echo Generando $@
	mkdir -p $(BINPATH)
	$(CC) $(CFLAGS) $< -o $@

$(BINPATH)bitarray.o: $(SRCPATH)comunes/bitarray.c
	@echo Generando $@
	mkdir -p $(BINPATH)
	$(CC) $(CFLAGS) $< -o $@

$(BINPATH)config.o: $(SRCPATH)comunes/config.c
	@echo Generando $@
	mkdir -p $(BINPATH)
	$(CC) $(CFLAGS) $< -o $@

$(BINPATH)error.o: $(SRCPATH)comunes/error.c
	@echo Generando $@
	mkdir -p $(BINPATH)
	$(CC) $(CFLAGS) $< -o $@

$(BINPATH)log.o: $(SRCPATH)comunes/log.c
	@echo Generando $@
	mkdir -p $(BINPATH)
	$(CC) $(CFLAGS) $< -o $@

$(BINPATH)memory.o: $(SRCPATH)comunes/memory.c
	@echo Generando $@
	mkdir -p $(BINPATH)
	$(CC) $(CFLAGS) $< -o $@

$(BINPATH)process.o: $(SRCPATH)comunes/process.c
	@echo Generando $@
	mkdir -p $(BINPATH)
	$(CC) $(CFLAGS) $< -o $@

$(BINPATH)string.o: $(SRCPATH)comunes/string.c
	@echo Generando $@
	mkdir -p $(BINPATH)
	$(CC) $(CFLAGS) $< -o $@

$(BINPATH)temporal.o: $(SRCPATH)comunes/temporal.c
	@echo Generando $@
	mkdir -p $(BINPATH)
	$(CC) $(CFLAGS) $< -o $@

$(BINPATH)txt.o: $(SRCPATH)comunes/txt.c
	@echo Generando $@
	mkdir -p $(BINPATH)
	$(CC) $(CFLAGS) $< -o $@

$(BINPATH)dictionary.o: $(SRCPATH)comunes/collections/dictionary.c
	@echo Generando $@
	mkdir -p $(BINPATH)
	$(CC) $(CFLAGS) $< -o $@

$(BINPATH)list.o: $(SRCPATH)comunes/collections/list.c
	@echo Generando $@
	mkdir -p $(BINPATH)
	$(CC) $(CFLAGS) $< -o $@

$(BINPATH)queue.o: $(SRCPATH)comunes/collections/queue.c
	@echo Generando $@
	mkdir -p $(BINPATH)
	$(CC) $(CFLAGS) $< -o $@

clean:
	@echo limpiando carpeta..
	rm $(BINPATH)*.bin $(BINPATH)*.o $(BINPATH)*.elf $(BINPATH)*.lst *.map $(SRC_NAME)