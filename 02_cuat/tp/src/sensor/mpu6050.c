#include <sys/types.h>
#include <stdio.h>

#include "../../inc/sensor/mpu6050.h"
#include "../../inc/sensor/dsp.h"

/**
 * @brief Convierte los datos planos q16 a variables fisicas float
 * 
 * @param dato_plano Datos planos sin formato
 * @param destino Datos fisicos
 */
void convertir_dato_fisico(sensor_datos_t *dato_plano, sensor_datos_fisicos_t *destino){
    q15_to_float((int16_t *)dato_plano, (float *)destino, sizeof(sensor_datos_t) / 2);
    bloque_ganancia_polarizacion_float(&destino->accel.x, &destino->accel.x , 1 / (float)SENSIBILIDAD_ACC   , 0.0             , 3);
    bloque_ganancia_polarizacion_float(&destino->temp   , &destino->temp    , 1 / (float)SENSIBILIDAD_TEMP  , ORDENADA_TEMP , 1);
    bloque_ganancia_polarizacion_float(&destino->gyro.x , &destino->gyro.x  , 1 / (float)SENSIBILIDAD_GYRO  , 0.0             , 3);
}

/**
 * @brief Imprime el dato en pantalla
 * 
 * @param dato 
 */
void imprimir_dato_plano(sensor_datos_t dato){
    printf("Ac lineal X: %d\n", dato.accel.x);
    printf("Ac lineal Y: %d\n", dato.accel.y);
    printf("Ac lineal Z: %d\n", dato.accel.z);
    printf("Temperatura: %d\n", dato.temp);
    printf("Ac angular X: %d\n", dato.gyro.x);
    printf("Ac angular Y: %d\n", dato.gyro.y);
    printf("Ac angular Z: %d\n", dato.gyro.z);
}

/**
 * @brief Imprime el dato en pantalla
 * 
 * @param dato 
 */
void imprimir_dato_fisico(sensor_datos_fisicos_t dato){
    printf("Ac lineal X: %f\n", dato.accel.x);
    printf("Ac lineal Y: %f\n", dato.accel.y);
    printf("Ac lineal Z: %f\n", dato.accel.z);
    printf("Temperatura: %f\n", dato.temp);
    printf("Ac angular X: %f\n", dato.gyro.x);
    printf("Ac angular Y: %f\n", dato.gyro.y);
    printf("Ac angular Z: %f\n", dato.gyro.z);
}
