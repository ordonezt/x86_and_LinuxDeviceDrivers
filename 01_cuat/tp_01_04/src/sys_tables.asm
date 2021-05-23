USE16

GLOBAL  CS_SEL, DS_SEL, SS_SEL, ES_SEL, GS_SEL, FS_SEL, SS_SELECTOR
GLOBAL  _gdtr

extern __TABLAS_SISTEMA_LMA

CS_SEL EQU 0b0000000000001000 ; Indice  = 1
                              ; TI      = 0 (GDT)
                              ; RPL     = 00 (Mayor privilegio)

DS_SEL EQU 0b0000000000010000 ; Indice = 2
                              ; TI      = 0 (GDT)
                              ; RPL     = 00 (Mayor privilegio)

ES_SEL EQU DS_SEL
GS_SEL EQU DS_SEL
FS_SEL EQU DS_SEL

SS_SEL EQU 0b0000000000011000 ; Indice = 3
                              ; TI      = 0 (GDT)
                              ; RPL     = 00 (Mayor privilegio)

; %define CS_SEL 0b0000000000001000
;                                 ; Indice  = 1
;                                 ; TI      = 0 (GDT)
;                                 ; RPL     = 00 (Mayor privilegio)

; %define DS_SEL 0b0000000000010000
;                                 ; Indice = 2
;                                 ; TI      = 0 (GDT)
;                                 ; RPL     = 00 (Mayor privilegio)

; %define ES_SEL DS_SEL
; %define GS_SEL DS_SEL
; %define FS_SEL DS_SEL

; %define SS_SEL 0b0000000000011000
;                                 ; Indice = 3
;                                 ; TI      = 0 (GDT)
;                                 ; RPL     = 00 (Mayor privilegio)

TABLAS_SISTEMA_LMA_SHADOW EQU __TABLAS_SISTEMA_LMA - 0xFFF00000 

section .tablas_sistema

GDT:

SELECTOR_NULO equ ($ - GDT)     ;Indice 0
    dq 0    ;Primer selector (8 bytes) en 0

CS_SELECTOR equ ($ - GDT)    ;Indice 8
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

DS_SELECTOR equ ($ - GDT)    ;Indice 16
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

SS_SELECTOR equ ($ - GDT)    ;Indice 16
                ;El limite debe ser 4kB, por eso usamos el G=0 y en limite ponemos 2^16 - 1 = 0xFFFF
    dw 0x0FFF   ;Limite 0: 0x0FFF
                ;Base: 0x2FFF8000
    dw 0x8000   ;Base 0: 0x8000
    db 0xFF     ;Base 1: 0xFF
    
    ;p dpl s b11 ED W A
    db 0b10010111
                ;A: 1 (Accedido, por defecto)
                ;W: 1 (Puede escribirse)
                ;ED: 1 (Se expande hacia posiciones menores)
                ;Bit 11: 0 (Segmento de datos)
                ;S: 1 (De codigo/datos)
                ;DPL: 00 (Privilegio nivel 0)
                ;P: 1 (Presente)
    
    db 0b01000000
                ;Limite 1: 0x0
                ;Limite: 64kB
                ;AVL: 0 (No tiene uso)
                ;L: 0 (Como no estamos en IA-32e no tiene sentido)
                ;D_B: 1 (Segmento de 32 bits)
                ;G: 0 (El limite esta en bytes)
    
    db 0x2F     ;Base 2: 0x2F
                ;Base: 0x2FFF8FFF

GDT_LARGO EQU $-GDT

_gdtr:
    dw GDT_LARGO - 1 ;Limite
    dd TABLAS_SISTEMA_LMA_SHADOW


;Por que las tablas quedan en ROM? Si es mas lento