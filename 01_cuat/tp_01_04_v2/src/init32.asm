;Archivo: init32.asm
;Fecha: 19/05/2021
;Descripcion:

USE32

section .rom_init

%include "inc/tablas_sistema.asm" 

extern DS_SEL_16
extern __STACK_START
extern __rutinas_size, __kernel_size
extern __RUTINAS_VMA, __KERNEL_VMA
extern __RUTINAS_LMA, __KERNEL_LMA
extern __mi_memcpy_rom, __mi_memcpy, cargar_gdt
extern kernel_init
extern _gdtr_32, GDT_32

global init32
init32:

;Inicializo los selector de manera basica
    mov ax, DS_SEL_16
    mov ds, ax
    mov es, ax
    mov gs, ax
    mov fs, ax

;Inicializo la pila
    mov ax, DS_SEL_16
    mov ss, ax
    mov esp, __STACK_START     ;No hacer esto si tenes un segmento para el stack

;Copio las rutinas a RAM
    push ebp
    mov ebp,esp
    push __rutinas_size
    push __RUTINAS_VMA
    push __RUTINAS_LMA
    call __mi_memcpy_rom
    leave
    
;Inicializo la configuracion final de los selectores
    push    ebp
    mov     ebp,esp
    push    _gdtr_32
    push    GDT_32
    call    cargar_gdt
    leave
    lgdt    [_gdtr_32]

;Copio el kernel a RAM
    push ebp
    mov ebp,esp
    push __kernel_size
    push __KERNEL_VMA
    push __KERNEL_LMA
    call __mi_memcpy
    leave

    ;Salto al kernel
    jmp CS_SEL_32 : kernel_init
end_init32: