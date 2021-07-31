#ifndef __RUTINAS_H
#define __RUTINAS_H

#include "../inc/my_types.h"

#define ERROR_DEFECTO 0
#define EXITO 1

typedef unsigned char byte;
typedef unsigned long dword;

uint8_t __mi_memcpy(void* origen,void *destino,uint32_t length);
void tarea_1(void);

#endif /*__RUTINAS_H*/
