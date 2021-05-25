USE32

section .kernel

global kernel_init

kernel_init:
    
    ;Probando excepcion de division por cero
    xor ax,ax
    mov bx, 0
    div bx


    hlt
    
end_kernel:
