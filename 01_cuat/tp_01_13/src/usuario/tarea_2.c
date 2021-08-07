#include "../../inc/teclado.h"
#include "../../inc/pantalla.h"
#include "../../inc/tarea_2.h"
#include "../../inc/rutinas.h"

__attribute__(( section(".tarea_2")))
void tarea_2(void)
{
    uint64_t suma;
    uint8_t cadena_suma[21];

    while(1)
    {
        suma = suma_tabla_digitos_saturada_16();

        hex64_2_str(suma, cadena_suma);
        
        td3_print((uint8_t*)"Suma tarea 2: ", 5, 0);
        td3_print(cadena_suma, 5, strlen((uint8_t*)"Suma tarea 2: "));

        td3_halt();
    }
}