#ifndef __PIC_H
#define __PIC_H

#include "../inc/my_types.h"

#define PIC1_PUERTO_COMANDO     0x20
#define PIC1_PUERTO_DATOS       0x21

#define PIC2_PUERTO_COMANDO     0xA0
#define PIC2_PUERTO_DATOS       0xA1

#define INTERRUPCION_SYSTICK    0
#define INTERRUPCION_TECLADO    1

void pic_limpiar_interrupcion(uint8_t fuente);
void pic_limpiar_interrupciones(void);

#endif /*__PIC_H*/
