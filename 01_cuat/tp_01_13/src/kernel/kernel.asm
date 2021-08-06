USE32

section .kernel

global kernel_init, cambiar_contexto, paginacion_apagar, paginacion_encender
global get_cr3, borrar_cr0_ts, prender_cr0_ts, habilitar_TSS, syscall

extern main


kernel_init:
    
    ; ;Probando excepcion de division por cero
    ; xor ax,ax
    ; mov bx, 0
    ; div bx

    call main

    hlt
    
end_kernel:

;void* get_cr3(void);
get_cr3:
    mov eax, cr3
    ret

;void cambiar_contexto(contexto_tarea_t *tarea_futura, directorio_tabla_paginas_t *dtp);
cambiar_contexto:
    ; xchg bx, bx
    mov ebp, esp
    ;EBP + 4*0  = Direcc retorno
    ;EBP + 4*1  = Direccion de contexto de tarea futura
    ;EBP + 4*2  = Directorio de tabla de paginas de tarea futura
    mov edi, [ebp + 4*1]
    mov eax, [ebp + 4*2]
    mov cr3, eax

    ;mov ss,  [edi + 4*4]
    ; mov eax, [edi + 4*5]
    ; mov cr3, eax
    ;mov edi, [edi + 4*7]
    mov esi, [edi + 4*7]
    mov ebp, [edi + 4*8]
    mov esp, [edi + 4*9]
    mov ebx, [edi + 4*10]
    mov edx, [edi + 4*11]
    mov ecx, [edi + 4*12]
    mov eax, [edi + 4*13]

    mov es,  [edi + 4*0]
    mov gs,  [edi + 4*1]
    mov fs,  [edi + 4*2]
    ;mov ds,  [edi + 4*3]
    push eax
    mov eax, [edi + 4*3]
    mov edi, [edi + 4*6]
    mov ds, eax
    pop eax
    ;Hace falta que pushee esto??
    ; push dword [ebp + 4*17] ;eflags
    ; push dword [ebp + 4*16] ;cs
    ; push dword [ebp + 4*15] ;eip
    iret

paginacion_apagar:
    mov eax, cr0
    and eax, 0x7FFFFFFF
    mov cr0, eax
    ret

paginacion_encender:
    mov eax, cr0
    or  eax, 0x80000000
    mov cr0, eax
    ret

;void borrar_cr0_ts(void);
borrar_cr0_ts:
    push eax
    mov  eax, cr0
    and  eax, 0xFFFFFFF7
    mov  cr0, eax
    pop  eax
    ret

;void prender_cr0_ts(void);
prender_cr0_ts:
    push eax
    mov  eax, cr0
    or   eax, 0x8
    mov  cr0, eax
    pop  eax
    ret

;void habilitar_TSS(uint32_t selector_TSS);
habilitar_TSS:
    push eax
    mov eax, [esp + 8]
    ltr ax
    pop eax
    ret

;Seccion standard
section .std
;uint32_t syscall(uint32_t numero, uint32_t arg1, uint32_t arg2, uint32_t arg3);
syscall:
    push ebp
    mov ebp, esp

    push ebx
    push ecx
    push edx

    mov eax, [ebp + 8]
    mov ebx, [ebp + 12]
    mov ecx, [ebp + 16]
    mov edx, [ebp + 20]
    int 0x80

    pop edx
    pop ecx
    pop ebx
    pop ebp

    ret