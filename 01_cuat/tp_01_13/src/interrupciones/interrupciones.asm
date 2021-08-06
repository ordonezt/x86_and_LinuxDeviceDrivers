section .interrupciones
extern PIC0_IRQHandler_c, INT80_IRQHandler_c
global PIC0_IRQHandler, habilitar_interrupciones, deshabilitar_interrupciones, INT80_IRQHandler

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

INT80_IRQHandler:
    push edx
    push ecx
    push ebx
    push eax
    call INT80_IRQHandler_c
    add esp, 4*4
    iret

habilitar_interrupciones:
    ;Habilito las interrupciones
    sti
    ret

deshabilitar_interrupciones:
    cli
    ret