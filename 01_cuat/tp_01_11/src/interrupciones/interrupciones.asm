section .interrupciones
extern PIC0_IRQHandler_c
global PIC0_IRQHandler, habilitar_interrupciones, deshabilitar_interrupciones

;Este handler lo tengo que hacer si o si en asm porque
;el bendito gcc me pone codigo en el handler de C sin
;que yo pueda meter mano
PIC0_IRQHandler:
    ; xchg bx, bx
    pushad
    mov eax, cr3
    push eax
    push ss
    push ds
    push fs
    push gs
    push es

    cld
    call PIC0_IRQHandler_c
    add esp, 6*4
    popad
    iret

habilitar_interrupciones:
    ;Habilito las interrupciones
    sti
    ret

deshabilitar_interrupciones:
    cli
    ret