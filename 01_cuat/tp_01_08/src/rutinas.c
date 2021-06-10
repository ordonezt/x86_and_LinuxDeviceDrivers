#include "../inc/rutinas.h"
#include "../inc/teclado.h"
#include "../inc/pantalla.h"

__attribute__(( section(".rutinas")))
byte __mi_memcpy(dword* src,dword *dst,dword length)
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

__attribute__(( section(".tarea_1")))
void tarea_1(void)
{
    // while(1)
    // {
    //asm("xchg %bx,%bx");
    uint64_t aux;
    static uint16_t fila=10, columna=10;
    uint8_t cadena[11];

    aux = promedio_tabla_digitos();

    hex64_2_str(aux, cadena);
    my_printf(cadena, fila, columna);

    // columna += strlen(cadena) + 2;
    // fila++;
    // }
}