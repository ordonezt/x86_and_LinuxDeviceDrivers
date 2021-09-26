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

#include "../../inc/servidor/servidor.h"
#include "../../inc/comunes/config.h"

#define SENSOR_EJECUTABLE       "../sensor/sensor"
#define CONFIG_PATH             "../../tp.config"
#define BACKLOG_DEFAULT         20
#define CANT_CONEX_MAX_DEFAULT  100
#define VENTANA_FILTRO_DEFAULT  2

int main(void)
{
    t_config *config; //Para manejar el archivo de configuracion
    int backlog, cant_conex_maxima, ventana_filtro; //Variables de configuracion

    //Ignoro la señal SIGCHLD para evitar tener que esperar a los hijos y complicarla
    signal(SIGCHLD, SIG_IGN);

    //Leo el archivo de configuracion
    config = config_create(CONFIG_PATH);
    if(config != NULL)
    {
        //El archivo existe, lo leo.
        if(config_has_property(config, "CANTIDAD_CONEXIONES") == true)
            cant_conex_maxima = config_get_int_value(config, "CANTIDAD_CONEXIONES");
        else
            cant_conex_maxima = CANT_CONEX_MAX_DEFAULT;

        if(config_has_property(config, "BACKLOG") == true)
            backlog = config_get_int_value(config, "BACKLOG");
        else
            backlog = BACKLOG_DEFAULT;

        if(config_has_property(config, "VENTANA_FILTRO") == true)
            ventana_filtro = config_get_int_value(config, "VENTANA_FILTRO");
        else
            ventana_filtro = VENTANA_FILTRO_DEFAULT;
    }
    else 
    {
        //No existe el archivo de configuracion, utilizo valores por default
        printf("No existe el archivo bro!\n");
        backlog = BACKLOG_DEFAULT;
        cant_conex_maxima = CANT_CONEX_MAX_DEFAULT;
        ventana_filtro = VENTANA_FILTRO_DEFAULT;
    }
    printf("Cantidad maxima de conexiones: %d\n", cant_conex_maxima);
    printf("Backlog: %d\n", backlog);
    printf("Ventana del filtro: %d\n", ventana_filtro);

    //Creo el proceso del sensor
    //Lo libero con la señal SIGUSR1
    if(!fork())
        execlp(SENSOR_EJECUTABLE, SENSOR_EJECUTABLE, NULL);


    while(1)
    {
        printf("Soy el hilo principal [PID %d]\n", getpid());
        sleep(1);
    }
    exit(0);
}
