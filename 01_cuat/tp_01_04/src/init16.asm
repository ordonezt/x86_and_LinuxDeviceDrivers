;Archivo: init16.asm
;Fecha: 19/05/2021
;Descripcion:

USE16

section .rom_init

%include "inc/processor-flags.asm" 

extern _gdtr
extern CS_SEL
extern init32

global init16

init16:
    mov ax, 0xF000
    mov ds, ax
    ;Cargo la gdt
    lgdt    [_gdtr]
    
    ;Copiamos el contenido de CR0 en ax
    smsw    ax             ;Copio el CR0 en eax
    or      ax, X86_CR0_PE ;Pongo un 1 en el bit del modo protegido
    lmsw    ax             ;Copio lo que hice de nuevo en el CR0
    
    ;Salto y ya estoy en modo protegido
    jmp     dword CS_SEL : init32
end_init16: