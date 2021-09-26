#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>

#include "../../inc/sensor/sensor.h"

volatile sig_atomic_t salir, flag_cambio_config;

void sigusr1_handler(int sig){
    flag_cambio_config = 1;
}

/**
 * @brief Handler de la señal SIGTERM.
 * 
 * Le avisa al programa principal que hay que cerrar todo.
 * 
 * @param sig 
 */
void sigterm_handler(int sig){
    salir = 1;
}

//Recibe por una FIFO los parametros de configuracion... despues lo hacemo'
/**
 * Parametros:
 * Path
 * Ventana de filtro
 * */

/**
 * @brief Programa que maneja el acceso al sensor
 * 
 * @return int 
 */
int main(void)
{
    struct sigaction sa_term;//, sa_usr1;
    sensor_t sensor;
    char *sensor_path = SENSOR_PATH;

    salir = 0;
    sa_term.sa_handler = sigterm_handler;
    sa_term.sa_flags = 0; // or SA_RESTART
    sigemptyset(&sa_term.sa_mask);
    if (sigaction(SIGINT, &sa_term, NULL) == -1)
    {
        perror("sigaction\n");
        exit(1);
    }

    flag_cambio_config = 0;

    //Abro el sensor
    if(abrir_sensor(&sensor, sensor_path) == -1)
    {
        perror("No se pudo abrir el sensor\n");
        exit(1);
    }

    while(salir == false)
    {
        //Leer sensor
        if(leer_sensor(&sensor, &sensor.datos) <= 0)
        {
            perror("Error en la lectura del sensor\n");
            salir = 1;
        }
        else
        {
            //Procesar
            printf("Aceleracion x: %d LSB\n", sensor.datos.accel.x);
            printf("Aceleracion y: %d LSB\n", sensor.datos.accel.y);
            printf("Aceleracion z: %d LSB\n", sensor.datos.accel.z);
            printf("Temp: %d\n", sensor.datos.temp);
            printf("Gyro x: %d\n", sensor.datos.gyro.x);
            printf("Gyro y: %d\n", sensor.datos.gyro.y);
            printf("Gyro z: %d\n", sensor.datos.gyro.z);


            sleep(1);
        }
    }

    printf("\nCerrando el sensor...");
    if(cerrar_sensor(&sensor) == -1)
    {
        perror("\nError cerrando el sensor\n");
        exit(1);
    }
    printf("Listo\n");

    exit(0);
}

/**
 * @brief Abre el sensor con la configuracion especificada.
 * 
 * @param sensor Estructura de tipo sensor para el manejo del mismo.
 * @param path Ruta donde se encuentra el sensor.
 * @return int Retorna el file descriptor del sensor y -1 si hubo un error.
 */
int abrir_sensor(sensor_t *sensor, char path[])
{
    return sensor->fd = open(path, O_RDONLY);
}

/**
 * @brief Cierra el sensor.
 * 
 * @param sensor Estructura de tipo sensor para el manejo del mismo.
 * @return int 0 en exito, -1 en error.
 */
int cerrar_sensor(sensor_t *sensor)
{
    return close(sensor->fd);
}

/**
 * @brief Lee un dato del sensor.
 * 
 * @param sensor Estructura de tipo sensor para el manejo del mismo.
 * @param dato Variable para almacenar el dato leido.
 * @return int En exito retorna la cantidad de bytes leidos, cero si el dispositivo esta cerrado y -1 en error.
 */
int leer_sensor(sensor_t *sensor, sensor_datos_t *datos)
{
    return read(sensor->fd, datos, sizeof(datos[0]));
}

//Falta hacer fir_init
//Igual es medio al pedo, solo hay que hacer una media
typedef struct{
    const int16_t *coeficientes; //Si lo ordeno al revez (como cmsis) me queda mas linda la notacion
    uint16_t cantidad_taps; //Cantidad de coeficientes del filtro
    int16_t *estados; //Cantidad de taps -1
}fir_q15_t;

int fir_q15(fir_q15_t *filtro, int16_t entrada[], int16_t salida[], uint16_t cantidad)
{
    uint16_t i, j;

    if(filtro == NULL || filtro->coeficientes == NULL || filtro->cantidad_taps == 0 || filtro->estados == NULL ||
        entrada == NULL || salida == NULL ||
        cantidad == 0)
        return -1;

    memset(salida, 0, cantidad * sizeof(salida[0]));

    for(i=0; i < cantidad; i++)
    {
        for(j=0; j < filtro->cantidad_taps; j++)
            salida[i] += filtro->coeficientes[j] * filtro->estados[j];
        salida[i] += filtro->coeficientes[j] * entrada[i];
        
        desplazar_agregar_q15(filtro->estados, entrada[i], 1);
    }
}

void desplazar_agregar_q15(int16_t vector[], int16_t dato_nuevo, int16_t cantidad)
{
    uint16_t i;

    for(i=0; i < cantidad; i++)
        vector[i] = vector[i+1];
    vector[i] = dato_nuevo;
}