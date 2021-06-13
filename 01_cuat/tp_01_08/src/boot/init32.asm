;Archivo: init32.asm
;Fecha: 19/05/2021
;Descripcion:

USE32

section .rom_init

%include "inc/tablas_sistema.asm" 

extern DS_SEL_16
extern __KERNEL_PILA_INICIO
extern __rutinas_size, __kernel_size, __interrupciones_size, __tarea_1_size
extern __RUTINAS_VMA, __KERNEL_VMA, __INTERRUPCIONES_VMA, __TAREA_1_VMA
extern __RUTINAS_LMA, __KERNEL_LMA, __INTERRUPCIONES_LMA, __TAREA_1_LMA
extern __mi_memcpy_rom, __mi_memcpy, cargar_gdt, cargar_idt, init_pic, pic_deshabilitar_todo, paginacion_inicializar
extern kernel_init
extern _gdtr_32, GDT_32, _idtr_32, IDT_32

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
    mov esp, __KERNEL_PILA_INICIO     ;No hacer esto si tenes un segmento para el stack

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

;Copio las interrupciones a RAM
    push ebp
    mov ebp,esp
    push __interrupciones_size
    push __INTERRUPCIONES_VMA
    push __INTERRUPCIONES_LMA
    call __mi_memcpy
    leave

;Inicializo la configuracion de las interrupciones
    push    ebp
    mov     ebp,esp
    push    cs
    push    _idtr_32
    push    IDT_32
    call    cargar_idt
    leave
    lidt    [_idtr_32]


;Copio el kernel a RAM
    push ebp
    mov ebp,esp
    push __kernel_size
    push __KERNEL_VMA
    push __KERNEL_LMA
    call __mi_memcpy
    leave

;Copio la tarea 1 a RAM
    push ebp
    mov ebp,esp
    push __tarea_1_size
    push __TAREA_1_VMA
    push __TAREA_1_LMA
    call __mi_memcpy
    leave

;Inicializo los PIC
    call init_pic
    call pic_deshabilitar_todo

    call paginacion_inicializar

;Habilito las interrupciones
    sti

    ;Salto al kernel
    jmp CS_SEL_32 : kernel_init
end_init32: