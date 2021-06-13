USE16

GLOBAL  CS_SEL_16, DS_SEL_16, SS_SEL_16, ES_SEL_16, GS_SEL_16, FS_SEL_16, SS_SELECTOR_16
GLOBAL  _gdtr_16

extern __TABLAS_SISTEMA_16_LMA

CS_SEL_16 EQU 0b0000000000001000 ; Indice  = 1
                              ; TI      = 0 (GDT)
                              ; RPL     = 00 (Mayor privilegio)

DS_SEL_16 EQU 0b0000000000010000 ; Indice = 2
                              ; TI      = 0 (GDT)
                              ; RPL     = 00 (Mayor privilegio)

ES_SEL_16 EQU DS_SEL_16
GS_SEL_16 EQU DS_SEL_16
FS_SEL_16 EQU DS_SEL_16

SS_SEL_16 EQU DS_SEL_16
; SS_SEL EQU 0b0000000000011000 ; Indice = 3
;                               ; TI      = 0 (GDT)
;                               ; RPL     = 00 (Mayor privilegio)

TABLAS_SISTEMA_16_LMA_SHADOW EQU __TABLAS_SISTEMA_16_LMA - 0xFFF00000 

section .tablas_sistema_16

GDT_16:

gdt_nulo_16:
SELECTOR_NULO_16 equ ($ - GDT_16)     ;Indice 0
    dq 0    ;Primer selector (8 bytes) en 0

cs_gdt_16:
CS_SELECTOR_16 equ ($ - GDT_16)    ;Indice 8
                ;El limite debe ser 4GB, por eso usamos el G=1 y en limite ponemos(2^32 - 2^12) / 2^12 = 0xFFFFF
    dw 0xFFFF   ;Limite 0: 0xFFFF
    dw 0x0000   ;Base 0: 0x0000
    db 0x00     ;Base 1: 0x00
    
    db 0b10011001
                ;A: 1 (Accedido, por defecto)
                ;R: 0 (No puede ser leido)
                ;C: 0 (No ajustable, no puede ser llamado de otros segmentos menos privilegiados)
                ;Bit 11: 1 (Segmento de codigo)
                ;S: 1 (De codigo/datos)
                ;DPL: 00 (Privilegio nivel 0)
                ;P: 1 (Presente)
    
    db 0b11001111
                ;Limite 1: 0xF
                ;Limite: 4GB
                ;AVL: 0 (No tiene uso)
                ;L: 0 (Como no estamos en IA-32e no tiene sentido)
                ;D_B: 1 (Segmento de 32 bits)
                ;G: 1 (El limite esta en paquetes de 4kB)
    
    db 0x00     ;Base 2: 0x00
                ;Base: 0x00000000

ds_gdt_16:
DS_SELECTOR_16 equ ($ - GDT_16)    ;Indice 16
                ;El limite debe ser 4GB, por eso usamos el G=1 y en limite ponemos(2^32 - 2^12) / 2^12 = 0xFFFFF
    dw 0xFFFF   ;Limite 0: 0xFFFF
    dw 0x0000   ;Base 0: 0x0000
    db 0x00     ;Base 1: 0x00
    
    db 0b10010011
                ;A: 1 (Accedido, por defecto)
                ;W: 1 (Puede escribirse)
                ;ED: 0 (Se expande hacia posiciones superiores)
                ;Bit 11: 0 (Segmento de datos)
                ;S: 1 (De codigo/datos)
                ;DPL: 00 (Privilegio nivel 0)
                ;P: 1 (Presente)
    
    db 0b11001111
                ;Limite 1: 0xF
                ;Limite: 4GB
                ;AVL: 0 (No tiene uso)
                ;L: 0 (Como no estamos en IA-32e no tiene sentido)
                ;D_B: 1 (Segmento de 32 bits)
                ;G: 1 (El limite esta en paquetes de 4kB)
    
    db 0x00     ;Base 2: 0x00
                ;Base: 0x00000000
                
GDT_LARGO_16 EQU $-GDT_16

_gdtr_16:
    dw GDT_LARGO_16 - 1 ;Limite
    dd TABLAS_SISTEMA_16_LMA_SHADOW