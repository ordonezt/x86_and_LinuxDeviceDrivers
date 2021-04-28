;Programa hola mundo amd64
;%rax	System call	    %rdi	 %rsi	  %rdx	    %r10	  %r8	    %r9
; 1         sys_write    fd      *buff   count
; 60        sys_exit    error   

global _start

;Seccion .data, aca van todas las variables y datos
section .data
mensaje db 'Hola mundo!', '\n'
mensaje_len EQU ($ - mensaje)

SYS_WRITE EQU 1
SYS_EXIT EQU 60

STD_OUT EQU 1

;Seccion .text, aca va el codigo
section .text
_start:
    mov rax, SYS_WRITE  ;Pongo el numero de syscall (1 ---> sys_write) en el registro rax
    mov rdi, STD_OUT  ;El file descriptor es stdout
    mov rsi, mensaje ;En rdx pongo la direccion de memoria de la string mensaje
    mov rdx, mensaje_len
    syscall

    mov rax, SYS_EXIT   ;La syscall que queremos llamar es sys_exit para devolver el control
    mov rdi, 0  ;Retornamos sin error
    syscall