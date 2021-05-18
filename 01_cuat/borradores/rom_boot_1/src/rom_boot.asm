USE16

KB         EQU 1024
NOP_OPCODE EQU 0x90
codigo_len EQU (end - init16) ;Calculo cuanto ocupa el codigo que escribi

times (4*KB - codigo_len) db NOP_OPCODE
;Rellena 4KB - la ocupacion del codigo para llenar una imagen de 4KB con 
;operaciones NOP

init16:
    cli             ;Deshabilito interrupciones para que no me
                    ;cambien el segmento de codigo (aunque por
                    ;defecto estan deshabilitadas)
    jmp init16
align 16            ;Me aseguro de que complete la memoria para
                    ;que la proxima instruccion (no hay 
                    ;ninguna, pero estaria apuntada por end)
                    ;comience en una posicion multiplo de 16 bytes
end: