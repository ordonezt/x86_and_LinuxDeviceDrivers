#ifndef __DSP_H
#define __DSP_H

#include <sys/types.h>

/**
 * @brief Realiza la media aritmetica de un set de numeros enteros de 16 bits (1 signo, 15 magnitud)
 * 
 * @param resultado Posicion de memoria donde se alojara el resultado
 * @param datos Vector de datos
 * @param longitud Cantidad de datos del vector
 * @return int 0 exito, -1 error
 */
int media_q15(int16_t *resultado, int16_t datos[], int32_t longitud);

#endif /*__DSP_H*/