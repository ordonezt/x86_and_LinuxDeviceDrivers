;Archivo: reset.asm
;Fecha: 19/05/2021
;Descripcion: Rutina de reset del uP, se asegura de saltar a init16

USE16

;Defino la seccion de reset
section .reset_vector

;Etiqueta al codigo de modo real
extern init16

;Hago global la etiqueta "reset" para que el linker la pueda ver y entre ahi
global reset

;Ni bien arrancamos, limpiamos interrupciones y saltamos al codigo
reset:
        cli
        jmp init16
end_reset: