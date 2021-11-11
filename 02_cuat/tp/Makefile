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
CFLAGS = -c -Wall -pthread
IFLAGS = -mgeneral-regs-only #Flags para compilar los handler de interrupciones en C
LDFLAGS = -z max-page-size=0x01000 --oformat=binary -m elf_i386
LIBFLAGS = -lpthread
LDSCRIPT = linker.ld
ODFLAGS = -CprsSx --prefix-addresses
OFLAGS = -Ofast #Flags para optimizacion

#en objs guardo todos mis objetos elf
OBJS_G = 	$(BINPATH)dsp.o			$(BINPATH)bitarray.o 	$(BINPATH)config.o 		$(BINPATH)error.o	\
			$(BINPATH)log.o 		$(BINPATH)memory.o 		$(BINPATH)process.o 	$(BINPATH)string.o	\
			$(BINPATH)temporal.o 	$(BINPATH)txt.o 		$(BINPATH)dictionary.o 	$(BINPATH)list.o	\
			$(BINPATH)queue.o		$(BINPATH)sem.o			$(BINPATH)sharmem.o		$(BINPATH)sockets.o	\
			$(BINPATH)signal.o		$(BINPATH)mpu6050.o

OBJS_1 = 	$(BINPATH)servidor.o 	
OBJS_2 = 	$(BINPATH)sensor.o 	

SRC1_NAME = servidor
SRC2_NAME = sensor

#all: $(BINPATH)$(SRC_NAME).o
all: $(SRC1_NAME) $(SRC2_NAME)
	@echo Ejecutando aplicacion
	./$(SRC1_NAME)

servidor: $(OBJS_G) $(OBJS_1)
	@echo Generando $(SRC1_NAME)
	$(CC) $(OBJS_G) $(OBJS_1) -o $(SRC1_NAME) $(LIBFLAGS)
#	@echo Ejecutando $(SRC1_NAME)
#	./$(SRC1_NAME)

sensor: $(OBJS_G) $(OBJS_2)
	@echo Generando $(SRC2_NAME)
	$(CC) $(OBJS_G) $(OBJS_2) -o $(SRC2_NAME) $(LIBFLAGS)
#	@echo Ejecutando $(SRC1_NAME)
#	./$(SRC1_NAME)

$(BINPATH)servidor.o: $(SRCPATH)servidor/servidor.c
	@echo Generando $@
	mkdir -p $(BINPATH)
	$(CC) $(CFLAGS) $< -o $@

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

$(BINPATH)dsp.o: $(SRCPATH)sensor/dsp.c
	@echo Generando $@
	mkdir -p $(BINPATH)
	$(CC) $(CFLAGS) $(OFLAGS) $< -o $@ 
	$(CC) -S $< -o $(DOCPATH)dsp.asm

$(BINPATH)mpu6050.o: $(SRCPATH)sensor/mpu6050.c
	@echo Generando $@
	mkdir -p $(BINPATH)
	$(CC) $(CFLAGS) $< -o $@

clean:
	@echo limpiando carpeta..
	rm $(BINPATH)*.bin $(BINPATH)*.o $(BINPATH)*.elf $(BINPATH)*.lst *.map $(SRC1_NAME) $(SRC2_NAME)