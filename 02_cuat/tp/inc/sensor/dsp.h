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

/**
 * @brief Amplifica y polariza una señal float
 * 
 * @param entrada Datos de entrada
 * @param salida Destino
 * @param ganancia Constante de amplificacion
 * @param polarizacion Constante de polarizacion
 * @param longitud Cantidad de datos
 * @return int 0 exito, -1 error
 */
int bloque_ganancia_polarizacion_float(float entrada[], float salida[], float ganancia, float polarizacion, int longitud);

/**
 * @brief Convierte una señal q15 en float
 * 
 * @param entrada Datos q15
 * @param salida Destino
 * @param longitud Cantidad de datos
 * @return int 0 exito, -1 error
 */
int q15_to_float(int16_t entrada[], float salida[], int longitud);

#endif /*__DSP_H*/