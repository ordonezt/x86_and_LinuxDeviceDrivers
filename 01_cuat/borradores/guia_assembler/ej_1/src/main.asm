global _start
extern my_itoa
extern my_strlen
extern my_strrev

struc   timespec
        sec     resq    1
        nsec    resq    1
endstruc

section .data

    delay:  istruc  timespec
            at      sec,    dq      2
            at      nsec,   dq      0
            iend
            
    ;MENSAJES
    msg_pid db 'Hola mi PID es: '
    msg_pid_len EQU ($ - msg_pid)

    msg_sleep_1 db 'Me voy a dormir 2 segundos', 10
    msg_sleep_1_len EQU ($ - msg_sleep_1)

    msg_sleep_2 db 'Aca estoy despierto otra vez.... saliendo... adiós!!', 10
    msg_sleep_2_len EQU ($ - msg_sleep_2)

    msg_new_line db 10
    msg_new_line_len EQU ($ - msg_new_line)

    PID_string TIMES 10 db 0

    ;SYSCALLS
    SYS_WRITE       EQU 4
    SYS_EXIT        EQU 1
    SYS_GET_PID     EQU 20
    SYS_NANO_SLEEP  EQU 162

    ;FILE DESCRIPTORS
    STD_OUT EQU 1

section .text
_start:

    mov rax, SYS_WRITE      ;Pongo el numero de syscall (sys_write) en el registro rax
    mov rbx, STD_OUT        ;El file descriptor es stdout
    mov rcx, msg_pid        ;En rcx pongo la direccion de comienzo del mensaje
    mov rdx, msg_pid_len    ;En rdx pongo la longitud de la string mensaje
    int 0x80
    
    ;Obtengo el PID
    mov rax, SYS_GET_PID      ;Pongo el numero de syscall (sys_get_pid) en el registro rax
    int 0x80

    ;El resultado esta en rax, lo paso a string
    mov rdi, PID_string
    mov rsi, rax
    call my_itoa
    
    ;El resultado esta en rax, lo paso a rcx para imprimir
    mov rcx, rax
    
    ;Obtengo la longitud del mensaje y lo guardo en rdx
    mov rdi, rax
    call my_strlen
    mov rdx, rax

    ;Imprimo el PID
    mov rax, SYS_WRITE      ;Pongo el numero de syscall (sys_write) en el registro rax
    mov rbx, STD_OUT        ;El file descriptor es stdout
    ;mov rcx, msg_pid        ;En rcx pongo la direccion de comienzo del mensaje
    ;mov rdx, msg_pid_len    ;En rdx pongo la longitud de la string mensaje
    int 0x80


    mov rax, SYS_WRITE      ;Pongo el numero de syscall (sys_write) en el registro rax
    mov rbx, STD_OUT        ;El file descriptor es stdout
    mov rcx, msg_new_line   ;En rcx pongo la direccion de comienzo del mensaje
    mov rdx, msg_new_line_len;En rdx pongo la longitud de la string mensaje
    int 0x80
    
    mov rax, SYS_WRITE      ;Pongo el numero de syscall (sys_write) en el registro rax
    mov rbx, STD_OUT        ;El file descriptor es stdout
    mov rcx, msg_sleep_1    ;En rsi pongo la direccion de comienzo del mensaje
    mov rdx, msg_sleep_1_len;En rdx pongo la longitud de la string mensaje
    int 0x80

    mov rax,SYS_NANO_SLEEP    ;sys_nanosleep
    mov rbx,delay  ;Vamos a dormir por delay segundos
    xor rcx,rcx   ;Limpio rcx, no quiero saber el tiempo remanente
    int 0x80

    mov rax, SYS_WRITE      ;Pongo el numero de syscall (sys_write) en el registro rax
    mov rbx, STD_OUT        ;El file descriptor es stdout
    mov rcx, msg_sleep_2    ;En rsi pongo la direccion de comienzo del mensaje
    mov rdx, msg_sleep_2_len;En rdx pongo la longitud de la string mensaje
    int 0x80

    mov rax, SYS_EXIT   ;La syscall que queremos llamar es sys_exit para devolver el control
    mov rbx, 0  ;Retornamos sin error
    int 0x80