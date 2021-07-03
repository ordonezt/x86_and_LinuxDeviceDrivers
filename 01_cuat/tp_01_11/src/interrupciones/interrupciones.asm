section .interrupciones
extern PIC0_IRQHandler_c
global PIC0_IRQHandler

;Este handler lo tengo que hacer si o si en asm porque el bendito gcc me pone codigo en el handler de C sin que yo pueda meter mano
PIC0_IRQHandler:
    ;xchg bx, bx
    pushad
    cld
    call PIC0_IRQHandler_c
    popad
    iret