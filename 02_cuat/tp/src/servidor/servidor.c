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
#include <pthread.h>

#include "../../inc/servidor/servidor.h"
#include "../../inc/comunes/config.h"
#include "../../inc/comunes/ipc/sockets.h"
#include "../../inc/comunes/collections/list.h"

void destruir_cliente(void *cliente);
int atender_cliente(int *socket);
void sigterm_handler(int sig);


volatile sig_atomic_t salir;

int main(void)
{
    t_config *config; //Para manejar el archivo de configuracion
    int backlog, cant_conex_maxima, ventana_filtro, puerto; //Variables de configuracion
    int socket_recepcion, socket_cliente;
    struct sockaddr_in info_socket_recepcion, info_socket_cliente;
    struct sigaction sa_term;
    pid_t pid_productor;
    t_list *lista_clientes;
    cliente_t *nuevo_cliente;

    //Manejo de la señal sigterm
    salir = 0;
    sa_term.sa_handler = sigterm_handler;
    sa_term.sa_flags = 0; // or SA_RESTART
    sigemptyset(&sa_term.sa_mask);
    if (sigaction(SIGINT, &sa_term, NULL) == -1)
    {
        perror("sigaction\n");
        exit(1);
    }

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
    printf("Cantidad maxima de conexiones: %d\n", cant_conex_maxima);
    printf("Backlog: %d\n", backlog);
    printf("Ventana del filtro: %d\n", ventana_filtro);

    //Creo el proceso del sensor
    //Lo libero con la señal SIGUSR1
    pid_productor = fork();
    if(pid_productor == -1)
    {
        perror("fork");
        exit(1);
    }
    else if(pid_productor == 0)
        execlp(SENSOR_EJECUTABLE, SENSOR_EJECUTABLE, NULL);

    //Levanto el servidor
    socket_recepcion = crear_servidor(puerto, &info_socket_recepcion, backlog);
    if(socket_recepcion == -1)
    {
        perror("crear_servidor");
        //exit(1);
    }
    printf("Servidor creado:\n");
    printf("->\tSocket: [%d]\n->\tPuerto: [%d]\n", socket_recepcion, puerto);

    lista_clientes = list_create();
    
    while(salir == 0)
    {
        //printf("Soy el hilo principal [PID %d]\n", getpid());
        if(list_size(lista_clientes) < cant_conex_maxima)
        {
            socket_cliente = atender_conexion(socket_recepcion, &info_socket_cliente);
            if(socket_cliente <= 0)
            {
                perror("atender_cliente");
                salir = 1;
            }
            else
            {
                //Hago hilo y lo atiendo
                nuevo_cliente = (cliente_t*)malloc(sizeof(*nuevo_cliente));
                nuevo_cliente->socket = socket_cliente;
                memcpy(&nuevo_cliente->info_socket, &info_socket_cliente, sizeof(info_socket_cliente));
                pthread_create(&nuevo_cliente->thread, NULL, (void*)atender_cliente, (void*)&nuevo_cliente->socket);

                //Agrego el cliente a la lista
                list_add(lista_clientes, nuevo_cliente);
            }
        }
        else
            printf("No atiendo mas clientes, espera!\n");
        sleep(1);
    }

    printf("Cerrando todos los clientes...");
    list_destroy_and_destroy_elements(lista_clientes, destruir_cliente);
    printf("Listo\n");

    printf("Deteniendo proceso productor...");
    waitpid(pid_productor, NULL, 0);
    printf("Listo\n");

    printf("Cerrando el servidor...");
    cerrar_servidor(socket_recepcion);
    printf("Listo\n");


    exit(0);
}

/**
 * @brief Atiende el cliente
 * 
 * @param socket Socket de comunicacion con el cliente
 * @return int 0 exito, -1 error
 */
int atender_cliente(int *socket)
{
    int socket_id = *socket;

    printf("Se conecto un nuevo cliente [Socket: %d]\n", socket_id);

    while(1)//(salir == 0) //TODO esto tambien rompe todo
    {
        printf("Atendiendo\n");
        sleep(1);
    }

    exit(0);
}

/**
 * @brief Destruye la estructura "cliente_t"
 * 
 * @param cliente Cliente a destruir
 */
void destruir_cliente(void *cliente)
{
    //pthread_join(((cliente_t*)cliente)->thread, NULL);// TODO Porque me traba todo esto?????????
    free(cliente);
}

/**
 * @brief Handler de la señal SIGTERM.
 * 
 * Le avisa al programa principal que hay que cerrar todo.
 * 
 * @param sig 
 */
void sigterm_handler(int sig)
{
    salir = 1;
}
