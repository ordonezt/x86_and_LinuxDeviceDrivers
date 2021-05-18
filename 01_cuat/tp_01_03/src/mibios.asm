USE16

GLOBAL Entry

section .romInit

%define RAM_SEGMENT 0x0000
%define RAM_OFFSET  0x7C00
;%define RAM_OFFSET  0x00000
;%define RAM_OFFSET  0xF0000


;Longitud del codigo
code_size EQU endCpy - codigo

codigo:
idle: ;Esto va a estar en RAM
    hlt
    jmp idle

;Subrutina memcpy
;Compatible ABI32
;@param edi Direccion de destino
;@param esi Direccion de fuente
;@param ecx Cantidad de bytes a copiar
;@return eax Puntero a la copia o NULL si hubo error
memcpy:
    ;Limpiamos el flag de direccion para que avance en posiciones de memoria ascendentes
    cld
cpy_byte:
    mov al, [cs:si] ;Movemos el dato a al (para que funcione stosb)
    stosb           ;Copio el dato
    inc si          ;Incremento la fuente
    loop cpy_byte   ;cx--
    ;si ya termine de copiar armo el retorno
    ;COMO HAGO?

    ;Una vez que copie todo salto a esa posicion
    jmp retorno

init:
    mov edi, RAM_OFFSET
    mov esi, codigo
    mov ecx, code_size
    jmp memcpy

retorno:
    jmp RAM_SEGMENT : RAM_OFFSET
endCpy:
; ALIGN 16

section .resetVector

Entry:
init16:
    cli
    jmp init
;Rellenamos la imagen restante para alinearla a 16 bytes
ALIGN 16
end: