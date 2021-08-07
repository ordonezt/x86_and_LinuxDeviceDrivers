#include "../../inc/teclado.h"
#include "../../inc/pantalla.h"
#include "../../inc/tarea_2.h"
#include "../../inc/rutinas.h"

extern tabla_digitos_t tabla_digitos;
extern uint32_t tick;

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

        // //Trato de leer el kernel
        // td3_read((uint8_t*)0x122000, cadena_suma, 1);
        
        // //Trato de escribir el kernel
        // td3_read(&tabla_digitos, &tick, 1);

        td3_halt();
    }
}