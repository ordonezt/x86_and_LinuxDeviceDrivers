USE32

section .kernel

global kernel_init
extern main

kernel_init:
    
    ; ;Probando excepcion de division por cero
    ; xor ax,ax
    ; mov bx, 0
    ; div bx

    call main

    hlt
    
end_kernel:
