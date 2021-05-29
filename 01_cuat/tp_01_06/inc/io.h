#ifndef __IO_H
#define __IO_H

#include "../inc/my_types.h"

#define TECLADO_PUERTO_DATOS  0x60
#define TIMER_PUERTO_DATOS    0x40
#define TIMER_PUERTO_COMANDO  0x43

uint8_t puerto_io_leer(uint8_t puerto);
uint8_t puerto_io_escribir(uint8_t puerto, uint8_t dato);

#endif /* __IO_H */
