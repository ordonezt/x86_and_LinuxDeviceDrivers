section .rutinas

global puerto_io_leer, puerto_io_escribir

;uint8_t puerto_io_leer(uint8_t puerto)
puerto_io_leer:
    push ebp
    mov ebp, esp

    push edx

    mov edx, [ebp + 2 * 4]  ;Copio el argumento (direccion) en edx
    in  al, dx          ;Leo el dispositivo y guardo el resultado en eax (retorno)
    
    pop edx

    pop ebp

    ret

;uint8_t puerto_io_escribir(uint8_t puerto, uint8_t dato)
puerto_io_escribir:
    push ebp
    mov ebp, esp

    push edx
    push eax

    mov edx, [ebp + 2 * 4]  ;Copio el primer argumento (direccion) en edx
    mov eax, [ebp + 3 * 4]  ;Copio el segundo argumento (dato) en eax
    out dx, al          ;Escribo el dispositivo

    pop eax
    pop edx

    pop ebp

    ret

global get_cr2
get_cr2:
    mov eax, cr2
    ret