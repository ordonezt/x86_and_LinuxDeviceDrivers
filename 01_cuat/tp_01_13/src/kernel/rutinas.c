#include "../../inc/rutinas.h"
#include "../../inc/interrupciones.h"
#include "../../inc/main.h"

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

__attribute__(( section(".rutinas")))
void td3_halt(void)
{
    syscall(SYSCALL_HLT);
}