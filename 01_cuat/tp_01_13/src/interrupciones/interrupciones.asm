section .interrupciones
extern PIC0_IRQHandler_c, INT80_IRQHandler_c
global PIC0_IRQHandler, habilitar_interrupciones, deshabilitar_interrupciones, INT80_IRQHandler

;Este handler lo tengo que hacer si o si en asm porque
;gcc me pone codigo en el handler de C sin que yo 
;pueda meter mano
PIC0_IRQHandler:
    ;Guardo en la pila todos los registros (guardo el contexto)
    pushad
    mov eax, cr3
    push eax
    push ss
    push ds
    push fs
    push gs
    push es

    cld
    ;Una vez que ya guarde el contexto salto al handler de la interrupcion en C
    call PIC0_IRQHandler_c


    add esp, 6*4
    popad
    iret

;Este handler lo tengo que hacer si o si en asm porque
;gcc me pone codigo en el handler de C sin que yo 
;pueda meter mano y los parametros me llegan por registro
INT80_IRQHandler:
    push edx ;arg3
    push ecx ;arg2
    push ebx ;arg1
    push eax ;Tipo de syscall
    call INT80_IRQHandler_c
    add esp, 4*4
    iret

;Habilita las interrupciones
habilitar_interrupciones:
    sti
    ret

;Deshabilita las interrupciones
deshabilitar_interrupciones:
    cli
    ret