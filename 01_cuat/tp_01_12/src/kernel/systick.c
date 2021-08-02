#include "../../inc/pic.h"
#include "../../inc/systick.h"
#include "../../inc/io.h"

uint32_t tick __attribute__ ((section (".datos")));

__attribute__(( section(".rutinas"))) 
void systick_inicializar(void)
{
    puerto_io_escribir(TIMER_PUERTO_COMANDO, 0b00110100); //Configuracion
    puerto_io_escribir(TIMER_PUERTO_DATOS, PIT_PRESCALE & 0xFF); //Interrumpe cada 10,000151 ms
    puerto_io_escribir(TIMER_PUERTO_DATOS, (PIT_PRESCALE >> 8) & 0xFF); //Interrumpe cada 10,000151 ms
    
    //En realidad deberia leer las que estan seteadas de antes y jugar con eso
    puerto_io_escribir(PIC1_PUERTO_DATOS, 0xFC);
    tick = 0;
}

void systick_incrementar_tick(void)
{
    tick++;
}

uint32_t systick_get_tick(void)
{
    return tick;
}