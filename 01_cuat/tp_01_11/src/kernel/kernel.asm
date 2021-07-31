USE32

section .kernel

global kernel_init, cambiar_contexto
extern main

kernel_init:
    
    ; ;Probando excepcion de division por cero
    ; xor ax,ax
    ; mov bx, 0
    ; div bx

    call main

    hlt
    
end_kernel:

;void cambiar_contexto(contexto_tarea_t)
cambiar_contexto:
    ;xchg bx, bx
    mov ebp, esp
    ;EBP + 4*0  = Direcc retorno
    ;EBP + 4*1  = ES
    ;EBP + 4*2  = GS
    ;EBP + 4*3  = FS
    ;EBP + 4*4  = DS
    ;EBP + 4*5  = SS
    ;EBP + 4*6  = CR3
    ;EBP + 4*7  = EDI
    ;EBP + 4*8  = ESI
    ;EBP + 4*9  = EBP
    ;EBP + 4*10 = ESP
    ;EBP + 4*11 = EBX
    ;EBP + 4*12 = EDX
    ;EBP + 4*13 = ECX
    ;EBP + 4*14 = EAX
    ;EBP + 4*15 = EIP
    ;EBP + 4*16 = CS
    ;EBP + 4*17 = EFLAGS
    mov es,  [ebp + 4*1]
    mov gs,  [ebp + 4*2]
    mov fs,  [ebp + 4*3]
    mov ds,  [ebp + 4*4]
    mov ss,  [ebp + 4*5]
    mov eax, [ebp + 4*6]
    mov cr3, eax
    mov edi, [ebp + 4*7]
    mov esi, [ebp + 4*8]
    ;mov ebp, [ebp + 4*9]
    mov esp, [ebp + 4*10]
    mov ebx, [ebp + 4*11]
    mov edx, [ebp + 4*12]
    mov ecx, [ebp + 4*13]
    mov eax, [ebp + 4*14]
    ;Hace falta que pushee esto??
    ; push dword [ebp + 4*17] ;eflags
    ; push dword [ebp + 4*16] ;cs
    ; push dword [ebp + 4*15] ;eip
    mov ebp, [ebp + 4*9]
    iret