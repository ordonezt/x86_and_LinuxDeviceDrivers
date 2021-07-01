#include "../../inc/teclado.h"
#include "../../inc/systick.h"
#include "../../inc/main.h"
#include "../../inc/rutinas.h"
#include "../../inc/tarea_1.h"
#include "../../inc/tarea_2.h"
#include "../../inc/tarea_3.h"
#include "../../inc/tarea_4.h"

__attribute__(( section(".kernel")))
int main(void)
{
    uint32_t tick;

    //Inicializaciones
    teclado_inicializar();
    systick_inicializar();

    //Loop
    while(1)
    {
        tick = systick_get_tick();
        
        if((tick % PERIODO_TAREA_1) == 0)
            tarea_1();

        if((tick % PERIODO_TAREA_2) == 0)
            tarea_2();

        if((tick % PERIODO_TAREA_3) == 0)
            tarea_3();

        tarea_4();
    }
}

__attribute__(( section(".kernel")))
void ir_a_dormir(void)
{
    asm("hlt");
}