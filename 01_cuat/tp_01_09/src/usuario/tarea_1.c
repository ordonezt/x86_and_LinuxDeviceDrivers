#include "../../inc/teclado.h"
#include "../../inc/pantalla.h"

__attribute__(( section(".tarea_1")))
void tarea_1(void)
{
    // while(1)
    // {
    //asm("xchg %bx,%bx");
    uint64_t aux;
    //static uint8_t fila=10, columna=10; //Por que cuando se los paso a la funcion van como 0? PELIGROSOoOO. PREGUNTAR
    uint8_t cadena[11];

    aux = promedio_tabla_digitos();

    hex64_2_str(aux, cadena);
    my_printf(cadena, 1, 60);

    // columna += strlen(cadena) + 2;
    // fila++;
    // }
}