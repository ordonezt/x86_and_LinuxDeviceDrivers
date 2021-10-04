#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <pthread.h>

#include "../../inc/servidor/servidor.h"
#include "../../inc/sensor/sensor.h"
#include "../../inc/comunes/config.h"
#include "../../inc/comunes/string.h"
#include "../../inc/comunes/ipc/sockets.h"
#include "../../inc/comunes/ipc/sharmem.h"
#include "../../inc/comunes/ipc/sem.h"
#include "../../inc/comunes/ipc/signal.h"
#include "../../inc/comunes/collections/list.h"

#define control_semaforo_datos(id, accion)  control_semaforo((id), 0, (accion))

void destruir_cliente(void *cliente);
void* atender_cliente(void *datos_cliente);
void sigint_handler(int sig);
void sigusr2_handler(int sig);
int levantar_configuracion(srv_config_t *srv_config, char path[]);
int destruir_configuracion(srv_config_t srv_config);

volatile sig_atomic_t salir, refrescar_config;

int main(void)
{
    srv_config_t config;
    int socket_recepcion, socket_cliente;
    struct sockaddr_in info_socket_recepcion, info_socket_cliente;
    struct sigaction sa_term;
    signal_t sig_int, sig_usr2;
    pid_t pid_productor;
    t_list *lista_clientes;
    cliente_t *nuevo_cliente;
    int sem_id, shm_id;
    key_t llave;
    datos_compartidos_t *mem_compartida;

    //Creo la llave para los ipc
    llave = ftok(CONFIG_PATH, 'T');
    if(llave < 0){
        perror("Creación de llave");
        exit(1);
    }

    //Creo el set de semaforos
    sem_id = crear_semaforo(CANTIDAD_SEMAFOROS, llave);

/* +++++++++++SEÑALES++++++++++++ */
    //Manejo de la señal sigterm (Le avisa al programa principal que hay que cerrar todo)
    salir = 0;
    if(atrapar_signal(&sig_int, sigint_handler, SIGINT)){
        perror("atrapar_signal");
        exit(1);
    }

    //Manejo de la señal sigusr2 (Avisa que hay que refrescar las configuraciones)
    refrescar_config = 0;
    if(atrapar_signal(&sig_usr2, sigusr2_handler, SIGUSR2)){
        perror("atrapar_signal");
        exit(1);
    }

    //Ignoro la señal SIGCHLD para evitar tener que esperar a los hijos y complicarla
    signal(SIGCHLD, SIG_IGN);
/* +++++++++++FIN SEÑALES+++++++++++ */

    //Leo el archivo de configuracion
    config.config = NULL;
    levantar_configuracion(&config, CONFIG_PATH);
    
    //Creo la memoria compartida y la inicializo
    shm_id = crear_shmem(&mem_compartida, llave, sizeof(mem_compartida[0]));
    if(shm_id <= 0){
        perror("crear_shmem");
        exit(1);
    }
    mem_compartida->ventana_filtro = config.ventana_filtro;

    //Creo el proceso del sensor
    //Lo libero con la señal SIGUSR1 //TODO ??
    pid_productor = fork();
    if(pid_productor == -1)
    {
        perror("fork");
        exit(1);
    }
    else if(pid_productor == 0)
        execlp(SENSOR_EJECUTABLE, SENSOR_EJECUTABLE                     //argv[0]
                                , string_itoa(sem_id)                   //argv[1]
                                , string_itoa(config.ventana_filtro)    //argv[2]
                                , NULL);

    //Levanto el servidor
    socket_recepcion = crear_servidor(config.puerto, &info_socket_recepcion, config.backlog);
    if(socket_recepcion == -1)
    {
        perror("crear_servidor");
        //exit(1);
    }

    //Preparo la lista de clientes
    lista_clientes = list_create();
    
    printf("[PID %d]\tServidor creado con exito\n", getpid());
    printf("\t\tCantidad maxima de conexiones: %d\n", config.cant_conex_maxima);
    printf("\t\tBacklog: %d\n", config.backlog);
    printf("\t\tVentana del filtro: %d\n", config.ventana_filtro);

    while(salir == 0)
    {
        //printf("Soy el hilo principal [PID %d]\n", getpid());
        if(list_size(lista_clientes) < config.cant_conex_maxima)
        {
            socket_cliente = atender_conexion(socket_recepcion, &info_socket_cliente);
            if(socket_cliente <= 0)
            {
                perror("atender_cliente");
                //salir = 1;
            }
            else
            {
                //TODO Encapsular
                //Hago hilo y lo atiendo
                nuevo_cliente = (cliente_t*)malloc(sizeof(*nuevo_cliente));
                nuevo_cliente->socket = socket_cliente;
                nuevo_cliente->semaforo = sem_id;
                nuevo_cliente->mem_compartida = mem_compartida;
                memcpy(&nuevo_cliente->info_socket, &info_socket_cliente, sizeof(info_socket_cliente));
                pthread_create(&nuevo_cliente->thread, NULL, atender_cliente, (void*)nuevo_cliente);

                //Agrego el cliente a la lista
                list_add(lista_clientes, nuevo_cliente);
            }
        }
        else
            printf("No atiendo mas clientes, espera!\n");
        
        if(refrescar_config){
            int ventana_filtro_anterior = config.ventana_filtro;
            
            //Re-leo el archivo de configuracion
            levantar_configuracion(&config, CONFIG_PATH);
            
            //Si es necesario le aviso al productor el cambio
            if(ventana_filtro_anterior != config.ventana_filtro){
                mem_compartida->ventana_filtro = config.ventana_filtro;
                kill(pid_productor, SIGUSR2);
            }
        }

        sleep(1);
    }

//TODO Encapsular
    fflush(stdout);
    printf("Liberando configuraciones........");
    destruir_configuracion(config);
    printf("Listo\n");

    printf("Cerrando todos los clientes......");
    list_clean_and_destroy_elements(lista_clientes, destruir_cliente);
    printf("Listo\n");

    printf("Deteniendo proceso productor.....");
    waitpid(pid_productor, NULL, 0);
    printf("Listo\n");

    printf("Cerrando el servidor.............");
    cerrar_servidor(socket_recepcion);
    printf("Listo\n");

    printf("Borrando el set de semaforos.....");
    destruir_semaforo(sem_id);
    printf("Listo\n");

    printf("Borrando la memoria compartida...");
    destruir_shmem(shm_id, mem_compartida);
    printf("Listo\n");

    exit(0);
}

/**
 * @brief Atiende el cliente
 * 
 * @param args Datos necesarios para atender al cliente.
 * Socket, semaforo a datos, etc.
 * @return void* 0 exito, -1 error
 */
void* atender_cliente(void *datos_cliente)
{
    int socket_id, sem_id;
    sensor_datos_t *sensor;
    
    socket_id   = ((cliente_t*)datos_cliente)->socket;
    sem_id      = ((cliente_t*)datos_cliente)->semaforo;
    sensor      = &((cliente_t*)datos_cliente)->mem_compartida->datos_filtrados;

    printf("Se conecto un nuevo cliente [Socket: %d] [Semaforo: %d] [Memoria %d]\n", socket_id, sem_id, sensor);

    while(salir == 0)
    {
        if(control_semaforo_datos(sem_id, SEM_TAKE))
        {
            perror("control_semaforo_datos");
            pthread_exit((void*)1);
        }
        printf("Tome el semaforo atendiendo un cliente\n");
        //Procesar
        printf("Aceleracion x: %d LSB\n", sensor->accel.x);
        printf("Aceleracion y: %d LSB\n", sensor->accel.y);
        printf("Aceleracion z: %d LSB\n", sensor->accel.z);
        printf("Temp: %d\n", sensor->temp);
        printf("Gyro x: %d\n", sensor->gyro.x);
        printf("Gyro y: %d\n", sensor->gyro.y);
        printf("Gyro z: %d\n", sensor->gyro.z);
        //sleep(3);
        if(control_semaforo_datos(sem_id, SEM_FREE))
        {
            perror("control_semaforo_datos");
            pthread_exit((void*)1);
        }
        printf("Solte el semaforo atendiendo un cliente\n");

        sleep(1);
    }

    pthread_exit((void*)0);
}

/**
 * @brief Destruye la estructura "cliente_t"
 * 
 * @param cliente Cliente a destruir
 */
void destruir_cliente(void *cliente)
{
    pthread_join(((cliente_t*)cliente)->thread, NULL);
    free(cliente);
}

/**
 * @brief Handler de la señal SIGINT.
 * 
 * Le avisa al programa principal que hay que cerrar todo.
 * 
 * @param sig 
 */
void sigint_handler(int sig)
{
    salir = 1;
}

void sigusr2_handler(int sig){
    refrescar_config = 1;
}

int levantar_configuracion(srv_config_t *srv_config, char path[]){
    t_config *config;
    int backlog, cant_conex_maxima, ventana_filtro, puerto;

    config = srv_config->config;
    if(config != NULL)
        destruir_configuracion(*srv_config);

    //Leo el archivo de configuracion
    config = config_create(path);
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
        
        if(config_has_property(config, "PUERTO") == true)
            puerto = config_get_int_value(config, "PUERTO");
        else
            puerto = PUERTO_DEFAULT;
    }
    else 
    {
        //No existe el archivo de configuracion, utilizo valores por default
        printf("No existe el archivo bro!\n");
        backlog = BACKLOG_DEFAULT;
        cant_conex_maxima = CANT_CONEX_MAX_DEFAULT;
        ventana_filtro = VENTANA_FILTRO_DEFAULT;
        puerto = PUERTO_DEFAULT;
    }
    srv_config->backlog             = backlog;
    srv_config->cant_conex_maxima   = cant_conex_maxima;
    srv_config->ventana_filtro      = ventana_filtro;
    srv_config->puerto              = puerto;
    srv_config->config              = config;
}

int destruir_configuracion(srv_config_t srv_config){
    config_destroy(srv_config.config);
}