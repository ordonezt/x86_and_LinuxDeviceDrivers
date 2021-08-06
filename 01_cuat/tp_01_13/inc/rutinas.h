#ifndef __RUTINAS_H
#define __RUTINAS_H

#include "../inc/my_types.h"

#define ERROR_DEFECTO 0
#define EXITO 1

typedef unsigned char byte;
typedef unsigned long dword;

uint8_t __mi_memcpy(void* origen,void *destino,uint32_t length);
void td3_halt(void);
uint32_t td3_print(uint8_t cadena[], uint8_t fila, uint8_t columna);

uint64_t sumatoria_uint16_saturada(uint64_t datos[], uint32_t cantidad);
uint64_t sumatoria_uint64_saturada(uint64_t datos[], uint32_t cantidad);

#endif /*__RUTINAS_H*/
