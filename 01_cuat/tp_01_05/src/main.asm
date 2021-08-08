USE32

section .kernel

global kernel_init

extern _gdtr_32, GDT_32, forzar_SS

kernel_init:
    
    ;Si descomentas esta podes generar division por cero y doble falta
    ; ;Probando excepcion de division por cero
    ; xor ax,ax
    ; mov bx, 0
    ; div bx

    ;Excepcion SS
    ;Pone el segmento de stack como no presente
    push    GDT_32
    call    forzar_SS

    jmp kernel_init

    hlt
    
end_kernel:
