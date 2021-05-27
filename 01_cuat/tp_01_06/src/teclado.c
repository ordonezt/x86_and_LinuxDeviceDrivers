#include "../inc/teclado.h"
#include "../inc/io.h"
#include "../inc/pic.h"

#define TECLADO_DIR     ((uint32_t*)0x60)


__attribute__(( section(".rutinas"))) 
uint8_t teclado_get_tecla(void)
{
    return puerto_io_leer(TECLADO_DIR);
}

__attribute__(( section(".rutinas"))) 
void teclado_inicializar(void)
{
    asm("xchg %bx, %bx");
    //En realidad deberia leer las que estan seteadas de antes y jugar con eso
    puerto_io_escribir(PIC1_PUERTO_DATOS, 0xFD);
}