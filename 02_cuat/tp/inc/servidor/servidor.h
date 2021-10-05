#ifndef __SERVIDOR_H
#define __SERVIDOR_H

#include <stdbool.h>

#include "../../inc/comunes/config.h"
#include "../../inc/comunes/ipc/sockets.h"
#include "../../inc/sensor/sensor.h"

#define SENSOR_EJECUTABLE       "./sensor"  //Ruta al ejecutable del proceso productor
#define CONFIG_PATH             "./tp.config"   //Ruta al archivo de configuracion

#define BACKLOG_DEFAULT         20      //Cantidad de clientes en cola de espera
#define CANT_CONEX_MAX_DEFAULT  100     //Cantidad maxima de clientes simultaneos
#define VENTANA_FILTRO_DEFAULT  2       //Cantidad de valores a promediar
#define PUERTO_DEFAULT          3490    //Puerto a utilizar para el servidor

#define CANTIDAD_SEMAFOROS      10      //Cantidad de semaforos disponibles
#define N_SEMAFORO_DATOS         0      //Numero de semaforo del manejo de datos

typedef struct{
    //Variables de configuracion
    int backlog;
    int cant_conex_maxima;
    int ventana_filtro;
    int puerto;
    //Para manejar el archivo de configuracion
    t_config *config;
}srv_config_t;

typedef struct{
    sensor_datos_t datos_crudos;
    sensor_datos_t datos_filtrados;
    int ventana_filtro;
}datos_compartidos_t;

typedef struct{
    pid_t pid_hilo_principal;
    bool expiro;
    int socket;
    int semaforo;
    datos_compartidos_t *mem_compartida;
    struct sockaddr_in info_socket;
    pthread_t thread;
}cliente_t;

#endif /*__SERVIDOR_H*/