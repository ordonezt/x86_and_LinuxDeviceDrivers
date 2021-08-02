#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "../inc/my_types.h"

#define PIT_FREC        1193182//Hz
#define PIT_PRESCALE    11932

#define SYSTICK_FREC    100//((PIT_FREC / PIT_PRESCALE)) //100Hz No se porque la cuenta no anda

#define COUNT_10ms      (SYSTICK_FREC / 100) //No se porque ((int)(0.01*SYSTICK_FREC)) No anda
#define COUNT_20ms      (2 * COUNT_10ms)
#define COUNT_30ms      (3 * COUNT_10ms)


#define COUNT_100ms     (10 * COUNT_10ms)
#define COUNT_200ms     (2 * COUNT_100ms)
#define COUNT_500ms     (5 * COUNT_100ms)

void systick_inicializar(void);
void systick_incrementar_tick(void);
uint32_t systick_get_tick(void);

#endif  /*__SYSTICK_H*/