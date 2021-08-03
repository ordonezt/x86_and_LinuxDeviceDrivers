;Archivo: init32.asm
;Fecha: 19/05/2021
;Descripcion:

USE32

section .rom_init

%include "inc/tablas_sistema.asm" 

extern DS_SEL_16
extern __KERNEL_PILA_INICIO_FISICA

extern __rutinas_size, __kernel_size, __interrupciones_size
extern __tarea_1_text_size, __tarea_1_data_size, __tarea_1_rodata_size, __tarea_2_text_size, __tarea_2_data_size, __tarea_2_rodata_size
extern __tarea_3_text_size, __tarea_3_data_size, __tarea_3_rodata_size, __tarea_4_text_size, __tarea_4_data_size, __tarea_4_rodata_size

extern __RUTINAS_VMA_FISICA, __KERNEL_VMA_FISICA, __INTERRUPCIONES_VMA_FISICA

extern __TAREA_1_TEXT_VMA_FISICA, __TAREA_1_DATA_VMA_FISICA, __TAREA_1_RODATA_VMA_FISICA, __TAREA_2_TEXT_VMA_FISICA, __TAREA_2_DATA_VMA_FISICA, __TAREA_2_RODATA_VMA_FISICA
extern __TAREA_3_TEXT_VMA_FISICA, __TAREA_3_DATA_VMA_FISICA, __TAREA_3_RODATA_VMA_FISICA, __TAREA_4_TEXT_VMA_FISICA, __TAREA_4_DATA_VMA_FISICA, __TAREA_4_RODATA_VMA_FISICA

extern __RUTINAS_LMA, __KERNEL_LMA, __INTERRUPCIONES_LMA

extern __TAREA_1_TEXT_LMA, __TAREA_1_DATA_LMA, __TAREA_1_RODATA_LMA, __TAREA_2_TEXT_LMA, __TAREA_2_DATA_LMA, __TAREA_2_RODATA_LMA
extern __TAREA_3_TEXT_LMA, __TAREA_3_DATA_LMA, __TAREA_3_RODATA_LMA, __TAREA_4_TEXT_LMA, __TAREA_4_DATA_LMA, __TAREA_4_RODATA_LMA

extern __mi_memcpy_rom, __mi_memcpy, cargar_gdt, cargar_idt, init_pic, pic_deshabilitar_todo, paginacion_inicializar
extern kernel_init
extern _gdtr_32, GDT_32, _idtr_32, IDT_32

extern tareas_inicializar
extern tarea_1, tarea_2, tarea_3, tarea_4
extern __TAREA_1_PILA_INICIO_FISICA, __TAREA_2_PILA_INICIO_FISICA, __TAREA_3_PILA_INICIO_FISICA, __TAREA_4_PILA_INICIO_FISICA

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
    mov esp, __KERNEL_PILA_INICIO_FISICA     ;No hacer esto si tenes un segmento para el stack

;Copio las rutinas a RAM
    push ebp
    mov ebp,esp
    push __rutinas_size
    push __RUTINAS_VMA_FISICA
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
    push __INTERRUPCIONES_VMA_FISICA
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
    push __KERNEL_VMA_FISICA
    push __KERNEL_LMA
    call __mi_memcpy
    leave

; ;Copio la tarea 1 a RAM
;     push ebp
;     mov ebp,esp
;     push __tarea_1_size
;     push __TAREA_1_VMA_FISICA
;     push __TAREA_1_LMA
;     call __mi_memcpy
;     leave

;Copio el codigo de la tarea 1 a RAM
    push ebp
    mov ebp,esp
    push __tarea_1_text_size
    push __TAREA_1_TEXT_VMA_FISICA
    push __TAREA_1_TEXT_LMA
    call __mi_memcpy
    leave
;Copio los datos globales volatiles de la tarea 1 a RAM
    push ebp
    mov ebp,esp
    push __tarea_1_data_size
    push __TAREA_1_DATA_VMA_FISICA
    push __TAREA_1_DATA_LMA
    call __mi_memcpy
    leave
;Copio los datos constantes de la tarea 1 a RAM
    push ebp
    mov ebp,esp
    push __tarea_1_rodata_size
    push __TAREA_1_RODATA_VMA_FISICA
    push __TAREA_1_RODATA_LMA
    call __mi_memcpy
    leave


;Copio el codigo de la tarea 2 a RAM
    push ebp
    mov ebp,esp
    push __tarea_2_text_size
    push __TAREA_2_TEXT_VMA_FISICA
    push __TAREA_2_TEXT_LMA
    call __mi_memcpy
    leave
;Copio los datos globales volatiles de la tarea 2 a RAM
    push ebp
    mov ebp,esp
    push __tarea_2_data_size
    push __TAREA_2_DATA_VMA_FISICA
    push __TAREA_2_DATA_LMA
    call __mi_memcpy
    leave
;Copio los datos constantes de la tarea 2 a RAM
    push ebp
    mov ebp,esp
    push __tarea_2_rodata_size
    push __TAREA_2_RODATA_VMA_FISICA
    push __TAREA_2_RODATA_LMA
    call __mi_memcpy
    leave

;Copio el codigo de la tarea 3 a RAM
    push ebp
    mov ebp,esp
    push __tarea_3_text_size
    push __TAREA_3_TEXT_VMA_FISICA
    push __TAREA_3_TEXT_LMA
    call __mi_memcpy
    leave
;Copio los datos globales volatiles de la tarea 3 a RAM
    push ebp
    mov ebp,esp
    push __tarea_3_data_size
    push __TAREA_3_DATA_VMA_FISICA
    push __TAREA_3_DATA_LMA
    call __mi_memcpy
    leave
;Copio los datos constantes de la tarea 3 a RAM
    push ebp
    mov ebp,esp
    push __tarea_3_rodata_size
    push __TAREA_3_RODATA_VMA_FISICA
    push __TAREA_3_RODATA_LMA
    call __mi_memcpy
    leave

;Copio el codigo de la tarea 4 a RAM
    push ebp
    mov ebp,esp
    push __tarea_4_text_size
    push __TAREA_4_TEXT_VMA_FISICA
    push __TAREA_4_TEXT_LMA
    call __mi_memcpy
    leave
;Copio los datos globales volatiles de la tarea 4 a RAM
    push ebp
    mov ebp,esp
    push __tarea_4_data_size
    push __TAREA_4_DATA_VMA_FISICA
    push __TAREA_4_DATA_LMA
    call __mi_memcpy
    leave
;Copio los datos constantes de la tarea 4 a RAM
    push ebp
    mov ebp,esp
    push __tarea_4_rodata_size
    push __TAREA_4_RODATA_VMA_FISICA
    push __TAREA_4_RODATA_LMA
    call __mi_memcpy
    leave

;Inicializo los PIC
    call init_pic
    call pic_deshabilitar_todo

    ;call tareas_inicializar
    call inicializar_pila_tarea1
    call inicializar_pila_tarea2
    call inicializar_pila_tarea3
    call inicializar_pila_tarea4
    
    call paginacion_inicializar

; ;Habilito las interrupciones
;     sti
    push eax
    mov eax, cr0
    and eax, 0xFFFFFFFB
    mov cr0, eax
    pop eax

    ;Salto al kernel
    jmp CS_SEL_32 : kernel_init
end_init32:

inicializar_pila_tarea1:
    pushad
    
    ;cargo EFLAGS en el stack
    mov edi,__TAREA_1_PILA_INICIO_FISICA - 3     ;direccion del stack (dirección física, todavía no paginamos)
    mov eax,0x202             ;EFLAGS
    mov [edi],eax
    ;cargo CS en el stack
    sub edi,4
    mov eax,0x8             ;CS
    mov [edi],eax
    ;cargo el EIP en el stack
    sub edi,4
    mov eax,tarea_1             
    mov [edi],eax  

    popad
    ret

inicializar_pila_tarea2:
    pushad
    
    ;cargo EFLAGS en el stack
    mov edi,__TAREA_2_PILA_INICIO_FISICA - 3     ;direccion del stack (dirección física, todavía no paginamos)
    mov eax,0x202             ;EFLAGS
    mov [edi],eax
    ;cargo CS en el stack
    sub edi,4
    mov eax,0x8             ;CS
    mov [edi],eax
    ;cargo el EIP en el stack
    sub edi,4
    mov eax,tarea_2             
    mov [edi],eax  

    popad
    ret

inicializar_pila_tarea3:
    pushad
    
    ;cargo EFLAGS en el stack
    mov edi,__TAREA_3_PILA_INICIO_FISICA - 3     ;direccion del stack (dirección física, todavía no paginamos)
    mov eax,0x202             ;EFLAGS
    mov [edi],eax
    ;cargo CS en el stack
    sub edi,4
    mov eax,0x8             ;CS
    mov [edi],eax
    ;cargo el EIP en el stack
    sub edi,4
    mov eax,tarea_3             
    mov [edi],eax  

    popad
    ret

inicializar_pila_tarea4:
    pushad
    
    ;cargo EFLAGS en el stack
    mov edi,__TAREA_4_PILA_INICIO_FISICA - 3     ;direccion del stack (dirección física, todavía no paginamos)
    mov eax,0x202             ;EFLAGS
    mov [edi],eax
    ;cargo CS en el stack
    sub edi,4
    mov eax,0x8             ;CS
    mov [edi],eax
    ;cargo el EIP en el stack
    sub edi,4
    mov eax,tarea_4             
    mov [edi],eax  

    popad
    ret