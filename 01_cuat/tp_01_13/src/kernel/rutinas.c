#include "../../inc/rutinas.h"
#include "../../inc/interrupciones.h"
#include "../../inc/main.h"

#define syscall_hlt()           syscall(SYSCALL_HLT, 0, 0, 0)
#define syscall_print(X, Y, Z)  syscall(SYSCALL_PRINT, (uint32_t)(X), (uint32_t)(Y), (uint32_t)(Z))

__attribute__(( section(".rutinas")))
uint8_t __mi_memcpy(void* origen,void *destino,uint32_t length)
{
    uint8_t *dst = (uint8_t*)destino;
    uint8_t *src = (uint8_t*)origen;
    
    byte status = ERROR_DEFECTO;
    if(length>0)
    {
        while(length)
        {
            length--;
            *dst++=*src++;
        }
        status = EXITO;
    }
    return (status);
}

//Seccion standard
__attribute__(( section(".std")))
void td3_halt(void)
{
    syscall_hlt();
}

__attribute__(( section(".std")))
uint32_t td3_print(uint8_t cadena[], uint8_t fila, uint8_t columna)
{
    return syscall_print(cadena, fila, columna);
}