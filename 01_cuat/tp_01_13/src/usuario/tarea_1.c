#include "../../inc/teclado.h"
#include "../../inc/pantalla.h"
#include "../../inc/rutinas.h"
#include "../../inc/tarea_1.h"

__attribute__(( section(".tarea_1")))
void tarea_1(void)
{
    // while(1)
    // {
    //asm("xchg %bx,%bx");
    uint64_t direccion;
    uint32_t contenido;
    //static uint8_t fila=10, columna=10; //Por que cuando se los paso a la funcion van como 0? PELIGROSOoOO. PREGUNTAR
    uint8_t cadena_direccion[21], cadena_contenido[11];
    
    while(1)
    {
    //     direccion = promedio_tabla_digitos();
    // // if(direccion < DIRECCION_RAM_MAX)
    // // {
    //     contenido = direccion;//*((uint32_t*)((uint32_t)direccion));
    //     hex64_2_str(direccion, cadena_direccion);
    //     hex32_2_str(contenido, cadena_contenido);
        
    //     my_printf((uint8_t*)"Direccion: ", 0, 0);
    //     my_printf(cadena_direccion, 0, strlen((uint8_t*)"Direccion: "));

    //     my_printf((uint8_t*)"Contenido: ", 2, 0);
    //     my_printf(cadena_contenido, 2, strlen((uint8_t*)"Contenido: "));

    //     // contenido = 1/0;
    // // }
    // MAGIC_BREAKPOINT
        td3_print((uint8_t*)"Hola Mundo!", 5, 9);
        td3_halt();
    }

    //ir_a_dormir();

    // columna += strlen(cadena) + 2;
    // fila++;
    // }
}