#include "../inc/teclado.h"
#include "../inc/systick.h"

__attribute__(( section(".kernel")))
int main(void)
{
    //uint8_t a;
    //Inicializaciones
    teclado_inicializar();
    systick_inicializar();

    //Loop
    while(1)
    {
        //  a = 1/0;
    }
}