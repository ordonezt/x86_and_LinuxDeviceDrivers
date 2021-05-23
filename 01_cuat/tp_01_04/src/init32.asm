;Archivo: init32.asm
;Fecha: 19/05/2021
;Descripcion:

USE32

section .rom_init

extern CS_SEL, DS_SEL, ES_SEL, GS_SEL, FS_SEL, SS_SEL, SS_SELECTOR
extern __STACK_END, __STACK_START
extern __rutinas_size, __kernel_size
extern __RUTINAS_VMA, __KERNEL_VMA
extern __RUTINAS_LMA, __KERNEL_LMA
extern __mi_memcpy, kernel_init

global init32
init32:

;Inicializo selector de datos
    mov ax, DS_SEL
    mov ds, ax
    
    mov ax, ES_SEL
    mov es, ax
    
    mov ax, GS_SEL
    mov gs, ax

    mov ax, FS_SEL
    mov fs, ax

;Inicializo la pila
    mov ax, SS_SELECTOR
    mov ss, ax
    ;mov esp, __STACK_START     ;No hacer esto si tenes un segmento para el stack

;Copio las rutinas a RAM
    push ebp
    mov ebp,esp
    push __rutinas_size
    push __RUTINAS_VMA
    push __RUTINAS_LMA
    call __mi_memcpy
    leave

;Copio el kernel a RAM
    push ebp
    mov ebp,esp
    push __kernel_size
    push __KERNEL_VMA
    push __KERNEL_LMA
    call __mi_memcpy
    leave

;Salto al kernel
jmp CS_SEL : kernel_init
end_init32: