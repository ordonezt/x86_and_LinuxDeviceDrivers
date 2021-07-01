#include "../../inc/teclado.h"
#include "../../inc/pantalla.h"
#include "../../inc/tarea_3.h"

__attribute__(( section(".tarea_3")))
void tarea_3(void)
{
    uint64_t suma;
    uint8_t cadena_suma[21];

    // while(1)
    // {

        suma = suma_tabla_digitos();

        hex64_2_str(suma, cadena_suma);
        
        my_printf((uint8_t*)"Suma tarea 3: ", 10, 0);
        my_printf(cadena_suma, 10, strlen((uint8_t*)"Suma tarea 3: "));

        //ir_a_dormir();

    // }
}