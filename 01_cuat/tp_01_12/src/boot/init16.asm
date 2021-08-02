;Archivo: init16.asm
;Fecha: 19/05/2021
;Descripcion:

USE16

section .rom_init

%include "inc/processor-flags.asm" 

extern _gdtr_16
extern CS_SEL_16
extern __STACK_16_START
extern init32, init_screen


global init16, init_screen_return

init16:

    mov ax, 0xF000
    mov ds, ax
    
    ; ;Armo una pila de 16 bits
    ; mov sp, __STACK_16_START

    ;Cargo la gdt de 16
    lgdt    [_gdtr_16]
    
    ;Inicializo pantalla
    jmp init_screen
init_screen_return:

    ;Copiamos el contenido de CR0 en ax
    smsw    ax             ;Copio el CR0 en eax
    or      ax, X86_CR0_PE ;Pongo un 1 en el bit del modo protegido
    lmsw    ax             ;Copio lo que hice de nuevo en el CR0
    
    ;Salto y ya estoy en modo protegido
    jmp     dword CS_SEL_16 : init32
end_init16: