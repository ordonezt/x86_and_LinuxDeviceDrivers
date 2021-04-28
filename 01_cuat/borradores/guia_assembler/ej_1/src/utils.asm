global my_itoa
global my_strlen
global my_strrev


section .text

my_itoa:
;en edi recibo el puntero a memoria
;en esi recibo el entero

    ;registros que use para calculos y tengo que restaurar:
    ;rax, rbx, rcx, rdx
    ; push edx
    ; push ecx
    ; push 

    xor ecx, ecx ;limpio el registro contador
    mov eax, esi ;muevo el entero de entrada a eax para dividirlo
loop1:
    ;divido por 10 y me quedo con el resto en edx
    mov bx, 10   ;pongo el divisor va en rbx
    xor edx, edx ;limpio el registro edx, no lo necesito en la division
    div bx       ;divido edx:eax por bx
    
    ;me queda la division en eax y el resto en edx
    
    add dl, '0' ;transformo ese resto en ascii

    mov [rdi+rcx], dl ;lo guardo

    inc cl ;incremento ecx

    ;si eax es cero termine,si no, salto al inicio
    cmp eax, 0  ;comparo eax con cero
    jne loop1    ;salto

    ;Doy por terminada la string con un null
    mov [rdi+rcx], byte 0
    
    call my_strrev

    mov rax, rdi
    
    ret ;retorno
end_my_itoa:

my_strrev:
    ;en rdi tengo la string
    ;la devuelvo en rax tambien

    push rcx
    push rdx
    push rbx

    ;Averiguo la cantidad de caracteres de la string
    call my_strlen

    ;Tengo la cantidad + 1 en rax
    mov rbx, rax
    shr rbx, 1
    dec rax

    inc rdi ;POR QUE TENGO QUE HACER ESTO???
    
    xor rcx, rcx
loop_strrev:    
    mov dl, byte [rdi + rcx]
    mov dh, byte [rdi + rax]
    mov [rdi + rcx], dh
    mov [rdi + rax], dl

    inc rcx
    dec rax
    cmp rcx, rbx
    jne loop_strrev

    mov rax, rdi

    pop rbx
    pop rdx
    pop rcx

    ret
end_my_strrev:

my_strlen:
    ;en rdi tengo la string
    ;en rax pongo el resultado

    xor rax, rax

    dec rdi
loop_strlen:
    inc rdi
    inc rax
    cmp byte [rdi], 0
    jne loop_strlen

    sub rdi, rax
    dec rax

    ret
end_my_strlen: