#include "../../inc/teclado.h"
#include "../../inc/pantalla.h"
#include "../../inc/tarea_3.h"
#include "../../inc/rutinas.h"

__attribute__(( section(".tarea_3")))
void tarea_3(void)
{
    uint64_t suma;
    uint8_t cadena_suma[21];

    while(1)
    {
        suma = suma_tabla_digitos_saturada_64();

        hex64_2_str(suma, cadena_suma);
        
        td3_print((uint8_t*)"Suma tarea 3: ", 10, 0);
        td3_print(cadena_suma, 10, strlen((uint8_t*)"Suma tarea 3: "));

        td3_halt();
    }
}