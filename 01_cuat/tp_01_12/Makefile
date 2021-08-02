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
CFLAGS = -c -m32 -fno-stack-protector -fno-asynchronous-unwind-tables -Wall -fno-PIC
IFLAGS = -mgeneral-regs-only #Flags para compilar los handler de interrupciones en C
LDFLAGS = -z max-page-size=0x01000 --oformat=binary -m elf_i386

LDSCRIPT = linker.ld
ODFLAGS = -CprsSx --prefix-addresses
#en objs guardo todos mis objetos elf
OBJS = $(BINPATH)init32.elf $(BINPATH)reset.elf $(BINPATH)sys_tables_16.elf $(BINPATH)sys_tables_32.elf $(BINPATH)init16.elf $(BINPATH)main.elf  $(BINPATH)kernel.elf  $(BINPATH)rutinas.elf $(BINPATH)rutinas_rom.elf $(BINPATH)tablas_sistema.elf $(BINPATH)interrupciones.elf $(BINPATH)pic.elf $(BINPATH)teclado.elf $(BINPATH)io.elf  $(BINPATH)ring_buffer.elf  $(BINPATH)systick.elf  $(BINPATH)pantalla.elf  $(BINPATH)init_screen.elf  $(BINPATH)tarea_1.elf  $(BINPATH)tarea_2.elf  $(BINPATH)tarea_3.elf  $(BINPATH)tarea_4.elf  $(BINPATH)interrupciones_asm.elf $(BINPATH)dsp.elf

SRC_NAME = mi_bios

all: $(BINPATH)$(SRC_NAME).bin $(BINPATH)$(SRC_NAME).elf bochs
	mkdir -p doc bin
	clean
	mkdir -p doc
	hexdump -C $(BINPATH)$(SRC_NAME).bin > $(DOCPATH)$(SRC_NAME)_hexdump.txt
	objdump $(ODFLAGS) $(BINPATH)$(SRC_NAME).elf > $(DOCPATH)$(SRC_NAME)_objdump.txt
	readelf -a $(BINPATH)$(SRC_NAME).elf > $(DOCPATH)$(SRC_NAME)_readelf.txt


$(BINPATH)$(SRC_NAME).bin: $(OBJS)
	ld $(LDFLAGS) -T $(LDSCRIPT) $(OBJS) -o $@ -Map $(SRC_NAME).map

$(BINPATH)$(SRC_NAME).elf: $(OBJS)
	ld -z max-page-size=0x01000 -m elf_i386 -T $(LDSCRIPT) $(OBJS) -o $@

$(BINPATH)kernel.elf: $(SRCPATH)kernel/kernel.asm
	@echo Generando $@
	mkdir -p bin
	$(AC) $(AFLAGS) $< -o $@ -l $@.lst

$(BINPATH)dsp.elf: $(SRCPATH)kernel/dsp.asm
	@echo Generando $@
	mkdir -p bin
	$(AC) $(AFLAGS) $< -o $@ -l $@.lst

$(BINPATH)sys_tables_16.elf: $(SRCPATH)boot/sys_tables_16.asm
	@echo Generando $@
	mkdir -p bin
	$(AC) $(AFLAGS) $< -o $@ -l $@.lst

$(BINPATH)sys_tables_32.elf: $(SRCPATH)boot/sys_tables_32.asm
	@echo Generando $@
	mkdir -p bin
	$(AC) $(AFLAGS) $< -o $@ -l $@.lst

$(BINPATH)reset.elf: $(SRCPATH)boot/reset.asm
	@echo Generando $@
	mkdir -p bin
	$(AC) $(AFLAGS) $< -o $@ -l $@.lst

$(BINPATH)init16.elf: $(SRCPATH)boot/init16.asm
	@echo Generando $@
	mkdir -p bin
	$(AC) $(AFLAGS) $< -o $@ -l $@.lst

$(BINPATH)init32.elf: $(SRCPATH)boot/init32.asm
	@echo Generando $@
	mkdir -p bin
	$(AC) $(AFLAGS) $< -o $@ -l $@.lst

$(BINPATH)pic.elf: $(SRCPATH)interrupciones/pic.asm
	@echo Generando $@
	mkdir -p bin
	$(AC) $(AFLAGS) $< -o $@ -l $@.lst

$(BINPATH)interrupciones_asm.elf: $(SRCPATH)interrupciones/interrupciones.asm
	@echo Generando $@
	mkdir -p bin
	$(AC) $(AFLAGS) $< -o $@ -l $@.lst

$(BINPATH)io.elf: $(SRCPATH)kernel/io.asm
	@echo Generando $@
	mkdir -p bin
	$(AC) $(AFLAGS) $< -o $@ -l $@.lst

$(BINPATH)rutinas.elf: $(SRCPATH)kernel/rutinas.c
	@echo Generando $@
	mkdir -p bin
	$(CC) $(CFLAGS) $< -o $@

$(BINPATH)rutinas_rom.elf: $(SRCPATH)kernel/rutinas_rom.c
	@echo Generando $@
	mkdir -p bin
	$(CC) $(CFLAGS) $< -o $@

$(BINPATH)tablas_sistema.elf: $(SRCPATH)boot/tablas_sistema.c
	@echo Generando $@
	mkdir -p bin
	$(CC) $(CFLAGS) $< -o $@

$(BINPATH)interrupciones.elf: $(SRCPATH)interrupciones/interrupciones.c
	@echo Generando $@
	mkdir -p bin
#	$(CC) $(CFLAGS) $< -o $@
	$(CC) $(CFLAGS) $(IFLAGS) $< -o $@

$(BINPATH)teclado.elf: $(SRCPATH)kernel/teclado.c
	@echo Generando $@
	mkdir -p bin
	$(CC) $(CFLAGS) $< -o $@

$(BINPATH)ring_buffer.elf: $(SRCPATH)kernel/ring_buffer.c
	@echo Generando $@
	mkdir -p bin
	$(CC) $(CFLAGS) $< -o $@

$(BINPATH)main.elf: $(SRCPATH)kernel/main.c
	@echo Generando $@
	mkdir -p bin
	$(CC) $(CFLAGS) $< -o $@

$(BINPATH)systick.elf: $(SRCPATH)kernel/systick.c
	@echo Generando $@
	mkdir -p bin
	$(CC) $(CFLAGS) $< -o $@

$(BINPATH)pantalla.elf: $(SRCPATH)kernel/pantalla.c
	@echo Generando $@
	mkdir -p bin
	$(CC) $(CFLAGS) $< -o $@

$(BINPATH)init_screen.elf: $(SRCPATH)kernel/init_screen.asm
	@echo Generando $@
	mkdir -p bin
	$(AC) $(AFLAGS) $< -o $@ -l $@.lst

$(BINPATH)tarea_1.elf: $(SRCPATH)usuario/tarea_1.c
	@echo Generando $@
	mkdir -p bin
	$(CC) $(CFLAGS) $< -o $@

$(BINPATH)tarea_2.elf: $(SRCPATH)usuario/tarea_2.c
	@echo Generando $@
	mkdir -p bin
	$(CC) $(CFLAGS) $< -o $@

$(BINPATH)tarea_3.elf: $(SRCPATH)usuario/tarea_3.c
	@echo Generando $@
	mkdir -p bin
	$(CC) $(CFLAGS) $< -o $@

$(BINPATH)tarea_4.elf: $(SRCPATH)usuario/tarea_4.c
	@echo Generando $@
	mkdir -p bin
	$(CC) $(CFLAGS) $< -o $@

bochs:
	@echo llamando a bochs
	bochs -f bochsrc

clean:
	@echo limpiando carpeta..
	rm $(BINPATH)*.bin $(BINPATH)*.elf $(BINPATH)*.lst *.map