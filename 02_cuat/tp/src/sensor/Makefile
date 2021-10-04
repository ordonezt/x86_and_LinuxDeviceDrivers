.PHONY: clean
#TODO incorporar en un unico makefile para todo el proyecto
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
OBJS =  $(BINPATH)sensor.o 		$(BINPATH)bitarray.o 	$(BINPATH)config.o 		$(BINPATH)error.o	\
		$(BINPATH)log.o 		$(BINPATH)memory.o 		$(BINPATH)process.o 	$(BINPATH)string.o	\
		$(BINPATH)temporal.o 	$(BINPATH)txt.o 		$(BINPATH)dictionary.o 	$(BINPATH)list.o	\
		$(BINPATH)queue.o		$(BINPATH)sem.o			$(BINPATH)sharmem.o		$(BINPATH)sockets.o	\
		$(BINPATH)signal.o

SRC_NAME = sensor

#all: $(BINPATH)$(SRC_NAME).o
all: $(OBJS)
	@echo Generando $(SRC_NAME)
	$(CC) $(OBJS) -o $(SRC_NAME)
	@echo Ejecutando $(SRC_NAME)
	./$(SRC_NAME)

$(BINPATH)sensor.o: $(SRCPATH)sensor/sensor.c
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

$(BINPATH)sem.o: $(SRCPATH)comunes/ipc/sem.c
	@echo Generando $@
	mkdir -p $(BINPATH)
	$(CC) $(CFLAGS) $< -o $@

$(BINPATH)sharmem.o: $(SRCPATH)comunes/ipc/sharmem.c
	@echo Generando $@
	mkdir -p $(BINPATH)
	$(CC) $(CFLAGS) $< -o $@

$(BINPATH)sockets.o: $(SRCPATH)comunes/ipc/sockets.c
	@echo Generando $@
	mkdir -p $(BINPATH)
	$(CC) $(CFLAGS) $< -o $@

$(BINPATH)signal.o: $(SRCPATH)comunes/ipc/signal.c
	@echo Generando $@
	mkdir -p $(BINPATH)
	$(CC) $(CFLAGS) $< -o $@
	
clean:
	@echo limpiando carpeta..
	rm $(BINPATH)*.bin $(BINPATH)*.o $(BINPATH)*.elf $(BINPATH)*.lst *.map $(SRC_NAME)