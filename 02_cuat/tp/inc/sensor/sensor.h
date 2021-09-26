#ifndef __SENSOR_H
#define __SENSOR_H

#include <stdint.h>

#define SENSOR_PATH "/dev/urandom"

typedef struct{
    struct aceleracion{
        uint16_t x;
        uint16_t y;
        uint16_t z;
    }accel;

    int16_t temp;

    struct gyro{
        uint16_t x;
        uint16_t y;
        uint16_t z;
    }gyro;
}sensor_datos_t;

typedef struct{
    int fd;
    sensor_datos_t datos;
    char path[];
}sensor_t;

/**
 * @brief Abre el sensor con la configuracion especificada.
 * 
 * @param sensor Estructura de tipo sensor para el manejo del mismo.
 * @param path Ruta donde se encuentra el sensor.
 * @return int Retorna el file descriptor del sensor y -1 si hubo un error.
 */
int abrir_sensor(sensor_t *sensor, char path[]);

/**
 * @brief Cierra el sensor.
 * 
 * @param sensor Estructura de tipo sensor para el manejo del mismo.
 * @return int 0 en exito, -1 en error.
 */
int cerrar_sensor(sensor_t *sensor);

/**
 * @brief Lee un dato del sensor.
 * 
 * @param sensor Estructura de tipo sensor para el manejo del mismo.
 * @param dato Variable para almacenar el dato leido.
 * @return int En exito retorna la cantidad de datos leidos, cero si el dispositivo esta cerrado y -1 en error.
 */
int leer_sensor(sensor_t *sensor, sensor_datos_t *datos);

#endif /*__SENSOR_H*/