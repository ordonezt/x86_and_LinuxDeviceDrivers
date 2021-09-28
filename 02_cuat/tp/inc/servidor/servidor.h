#ifndef __SERVIDOR_H
#define __SERVIDOR_H

#include "../../inc/comunes/ipc/sockets.h"

#define SENSOR_EJECUTABLE       "../sensor/sensor"
#define CONFIG_PATH             "../../tp.config"
#define BACKLOG_DEFAULT         20
#define CANT_CONEX_MAX_DEFAULT  100
#define VENTANA_FILTRO_DEFAULT  2
#define PUERTO_DEFAULT          3490

typedef struct{
    int socket;
    struct sockaddr_in info_socket;
    pthread_t thread;
}cliente_t;

#endif /*__SERVIDOR_H*/