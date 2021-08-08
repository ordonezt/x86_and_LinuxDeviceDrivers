#include "../../inc/teclado.h"
#include "../../inc/pantalla.h"
#include "../../inc/rutinas.h"
#include "../../inc/tarea_1.h"

__attribute__(( section(".tarea_1")))
void tarea_1(void)
{
    uint64_t direccion;
    uint32_t contenido;
    uint8_t cadena_direccion[22], cadena_contenido[11];
    
    while(1)
    {
        direccion = promedio_tabla_digitos();
    // if(direccion < DIRECCION_RAM_MAX)
    // {
        contenido = direccion;//*((uint32_t*)((uint32_t)direccion));
        hex64_2_str(direccion, cadena_direccion);
        hex32_2_str(contenido, cadena_contenido);
        
        td3_print((uint8_t*)"Direccion: ", 0, 0);
        td3_print(cadena_direccion, 0, strlen((uint8_t*)"Direccion: "));

        td3_print((uint8_t*)"Contenido: ", 2, 0);
        td3_print(cadena_contenido, 2, strlen((uint8_t*)"Contenido: "));

        // contenido = 1/0;
    // }
        td3_halt();
    }
}