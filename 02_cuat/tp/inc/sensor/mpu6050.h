#ifndef __MPU6050_H
#define __MPU6050_H

#include <stdint.h>

#define SENSIBILIDAD_ACC    16384//LSB/g

#define SENSIBILIDAD_TEMP   340//LSB/ºC
#define ORDENADA_TEMP       36.53//ºC

#define SENSIBILIDAD_GYRO   131//LSB/º/s

typedef struct{
    struct aceleracion{
        int16_t x;
        int16_t y;
        int16_t z;
    }accel;

    int16_t temp;

    struct gyro{
        int16_t x;
        int16_t y;
        int16_t z;
    }gyro;
}sensor_datos_t;

typedef struct{
    struct{
        float x;
        float y;
        float z;
    }accel;

    float temp;

    struct{
        float x;
        float y;
        float z;
    }gyro;
}sensor_datos_fisicos_t;

typedef struct{
    int fd;
    sensor_datos_t datos;
    char path[];
}sensor_t;

/**
 * @brief Convierte los datos planos q16 a variables fisicas float
 * 
 * @param dato_plano Datos planos sin formato
 * @param destino Datos fisicos
 */
void convertir_dato_fisico(sensor_datos_t *dato_plano, sensor_datos_fisicos_t *destino);

/**
 * @brief Imprime el dato en pantalla
 * 
 * @param dato 
 */
void imprimir_dato_plano(sensor_datos_t dato);

/**
 * @brief Imprime el dato en pantalla
 * 
 * @param dato 
 */
void imprimir_dato_fisico(sensor_datos_fisicos_t dato);

#endif /*__MPU6050_H*/
