#include "../../inc/rutinas.h"

//Copia un sector de memoria en otro
__attribute__(( section(".rutinas_rom"))) byte __mi_memcpy_rom(dword* src,dword *dst,dword length)
{
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