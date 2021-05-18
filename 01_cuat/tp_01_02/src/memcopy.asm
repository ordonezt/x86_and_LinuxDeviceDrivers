;Ponemos la primer instruccion en la direccion 0xFFFF0
;Esto es porque en el reset el IP esta en 0xFFF0 y los registros de cache ocultos del CS estan en 0xF000 y 0xFFFF0000
;Entonces arrancamos en (0xF000 << 4 + 0xFFF0) + 0xFFFF0000
;                       (0xF0000 + 0xFFF0)  + 0xFFFF0000
;                       (0xFFFF0) + 0xFFFF0000
                     
;Ponemos el inicio del programa en la direccion 0xFF000
;para rellenar la ROM de 4kB del enunciado
ORG 0xFF000

USE16

%define RAM_SEGMENT 0x0
%define RAM_OFFSET  0x7C00
;%define RAM_OFFSET  0x00000
;%define RAM_OFFSET  0xF0000

;Codigo de operacion del NOP
NOP_CODE EQU 0x90
;Longitud en bytes de la imagen
ROM_LEN EQU 4096
;Longitud del codigo
code_size EQU end - codigo

;Rellenamos con NOPs la imagen para depositar la primera
;instruccion en 0xFFFF0
times (ROM_LEN - code_size) db NOP_CODE

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

ALIGN 16
init16:
    cli
    jmp init
;Rellenamos la imagen restante para alinearla a 16 bytes
ALIGN 16
end: