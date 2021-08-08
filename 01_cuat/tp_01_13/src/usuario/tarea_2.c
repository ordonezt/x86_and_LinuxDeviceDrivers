#include "../../inc/teclado.h"
#include "../../inc/pantalla.h"
#include "../../inc/tarea_2.h"
#include "../../inc/rutinas.h"

extern tabla_digitos_t tabla_digitos;
extern uint32_t tick;
extern bool flag_AC;

__attribute__(( section(".tarea_2")))
void tarea_2(void)
{
    uint64_t suma;
    uint8_t cadena_suma[21];
    bool flag_aux = false;

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

        td3_read(&flag_AC, &flag_aux, 1);
        if(flag_aux == true)
            forzar_AC();

        td3_halt();
    }
}