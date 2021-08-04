USE32

GLOBAL  GDT_32, _gdtr_32
GLOBAL  IDT_32, _idtr_32

GLOBAL  __CS_BASE, __DS_BASE, __SS_BASE
GLOBAL  __CS_LIMITE, __DS_LIMITE, __SS_LIMITE

__CS_BASE   EQU     0x00000000
__CS_LIMITE EQU     0xFFFFFFFF
__DS_BASE   EQU     __CS_BASE
__DS_LIMITE EQU     __CS_LIMITE
__SS_BASE   EQU     __CS_BASE
__SS_LIMITE EQU     __CS_LIMITE

%define CANT_DESC_SEGM 5 ;Nulo + 2 codigo + 2 datos

section .tablas_sistema

_gdtr_32:
    dw 0
    dd 0
ALIGN 16
GDT_32:

_idtr_32 EQU (GDT_32 + 8 * CANT_DESC_SEGM)
IDT_32   EQU (_idtr_32 + 2 + 4 + 2) ;El ultimo 2 es para alinear
