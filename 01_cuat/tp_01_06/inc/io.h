#ifndef __IO_H
#define __IO_H

#include "../inc/my_types.h"

uint8_t puerto_io_leer(uint8_t puerto);
uint8_t puerto_io_escribir(uint8_t puerto, uint8_t dato);

#endif /* __IO_H */
