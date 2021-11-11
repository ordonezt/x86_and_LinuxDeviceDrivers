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

/*
 TODO FUNDAMENTAL: Implementar un identificador de muestra 
 (numero de muestra) para que los clientes sepan si la muestra
 que leyeron es la ultima o tienen que refrescarla, asi evito
 enviar data repetida
 */

/*
 TODO Hay un re bardo con el tema de la cantidad de bytes que envio y recibe el cliente.
 Si envio muy rapido lleno el buffer del cliente y los datos quedan recortados, haciendo
 que se desfase todo.
 Usar longitudes de trama fijas o usar dato de inicio y de fin de trama, si no se re complica.
 */
#define control_semaforo_datos(id, accion)  control_semaforo((id), 0, (accion))

void destruir_cliente(void *cliente);
void* atender_cliente(void *datos_cliente);
void sigint_handler(int sig);
void sigusr1_handler(int sig);
void sigusr2_handler(int sig);
int levantar_configuracion(srv_config_t *srv_config, char path[]);
void destruir_configuracion(srv_config_t srv_config);
void liberar_recursos(    pid_t pid_productor   , srv_config_t config
                        , t_list *lista_clientes, int socket_recepcion
                        , int sem_id            , int shm_id
                        , void *mem_cmp);
int crear_nuevo_cliente(  t_list *lista_clientes, int socket_cliente
                        , int sem_id            , void *mem_compartida
                        , struct sockaddr_in *info_socket_cliente);
int eliminar_clientes_expirados(t_list *lista);

volatile sig_atomic_t salir, refrescar_config, cliente_expiro;

int main(void)
{
    srv_config_t config;
    int socket_recepcion, socket_cliente;
    struct sockaddr_in info_socket_recepcion, info_socket_cliente;
    signal_t sig_int, sig_usr1, sig_usr2, sig_term;
    pid_t pid_productor;
    t_list *lista_clientes;
    int sem_id, shm_id;
    key_t llave;
    datos_compartidos_t *mem_compartida;

//TODO encapsular inicializacion

    //Creo la llave para los ipc
    llave = ftok(CONFIG_PATH, 'T');
    if(llave < 0){
        perror("Creación de llave");
        exit(EXIT_FAILURE);
    }

    //Creo el set de semaforos
    sem_id = crear_semaforo(CANTIDAD_SEMAFOROS, llave);

/* +++++++++++SEÑALES++++++++++++ */
    //Manejo de la señal sigint (Le avisa al programa principal que hay que cerrar todo)
    salir = 0;
    if(atrapar_signal(&sig_int, sigint_handler, SIGINT)){
        perror("atrapar_signal");
        exit(EXIT_FAILURE);
    }

    //Manejo de la señal sigterm (Le avisa al programa principal que hay que cerrar todo)
    if(atrapar_signal(&sig_term, sigint_handler, SIGTERM)){
        perror("atrapar_signal");
        exit(EXIT_FAILURE);
    }

    //Manejo de la señal sigusr1 (Avisa que un cliente cerro la conexion)
    cliente_expiro = 0;
    if(atrapar_signal(&sig_usr1, sigusr1_handler, SIGUSR1)){
        perror("atrapar_signal");
        exit(EXIT_FAILURE);
    }

    //Manejo de la señal sigusr2 (Avisa que hay que refrescar las configuraciones)
    refrescar_config = 0;
    if(atrapar_signal(&sig_usr2, sigusr2_handler, SIGUSR2)){
        perror("atrapar_signal");
        exit(EXIT_FAILURE);
    }

    //Ignoro la señal SIGCHLD para evitar tener que esperar a los hijos y complicarla
    signal(SIGCHLD, SIG_IGN);
/* +++++++++++FIN SEÑALES+++++++++++ */

    //Leo el archivo de configuracion
    config.config = NULL;
    levantar_configuracion(&config, CONFIG_PATH);
    
    //Creo la memoria compartida y la inicializo
    shm_id = crear_shmem((void**)&mem_compartida, llave, sizeof(mem_compartida[0]));
    if(shm_id < 0){
        perror("crear_shmem");
        exit(EXIT_FAILURE);
    }
    mem_compartida->ventana_filtro = config.ventana_filtro;

    //Creo el proceso del sensor
    pid_productor = fork();
    if(pid_productor == -1){
        perror("fork");
        exit(EXIT_FAILURE);
    }else if(pid_productor == 0)
        execlp(SENSOR_EJECUTABLE, SENSOR_EJECUTABLE                     //argv[0]
                                , string_itoa(sem_id)                   //argv[1]
                                , string_itoa(config.ventana_filtro)    //argv[2]
                                , NULL);

    //Levanto el servidor
    socket_recepcion = crear_servidor(config.puerto, &info_socket_recepcion, config.backlog);
    if(socket_recepcion == -1){
        perror("crear_servidor");
        salir = 1;
    }

    //Preparo la lista de clientes
    lista_clientes = list_create();
    
    printf("[PID %d]\tServidor creado con exito\n", getpid());
    printf("\t\tCantidad maxima de conexiones: %d\n", config.cant_conex_maxima);
    printf("\t\tBacklog: %d\n", config.backlog);
    printf("\t\tVentana del filtro: %d\n", config.ventana_filtro);

    while(salir == 0){
        //printf("Soy el hilo principal [PID %d]\n", getpid());
        if(list_size(lista_clientes) < config.cant_conex_maxima){
            socket_cliente = atender_conexion(socket_recepcion, &info_socket_cliente);
            if(socket_cliente <= 0){
                perror("atender_cliente");
                //salir = 1;
            }else{
                crear_nuevo_cliente(lista_clientes, socket_cliente, sem_id, mem_compartida, &info_socket_cliente);
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

        if(cliente_expiro){
            cliente_expiro = 0;
            printf("%d cliente eliminado\n", eliminar_clientes_expirados(lista_clientes));
            fflush(stdout);
        }

        sleep(1);
    }

    liberar_recursos(pid_productor, config, lista_clientes, socket_recepcion, sem_id, shm_id, mem_compartida);

    exit(EXIT_SUCCESS);
}

/**
 * @brief Crea e inicializa un hilo para atender un cliente.
 * Crea un hilo para atender al nuevo cliente y lo agrega a la lista de clientes.
 * 
 * @param lista_clientes Lista de los clientes.
 * @param socket_cliente Socket abierto para comunicarse con el cliente
 * @param sem_id ID del semaforo utilizado para sincronizar productor-consumidores
 * @param mem_compartida Puntero a la memoria compartida
 * @param info_socket_cliente Informacion de la conexion con el cliente
 * @return int 0 exito, -1 error
 */
int crear_nuevo_cliente(t_list *lista_clientes, int socket_cliente, int sem_id, void *mem_compartida, struct sockaddr_in *info_socket_cliente){
    cliente_t *nuevo_cliente;

    //Hago hilo y lo atiendo
    nuevo_cliente = (cliente_t*)malloc(sizeof(*nuevo_cliente));
    if(nuevo_cliente == NULL){
        perror("malloc");
        return -1;
    }
    nuevo_cliente->pid_hilo_principal = getpid();
    nuevo_cliente->expiro = false;
    nuevo_cliente->terminar = false;
    nuevo_cliente->socket = socket_cliente;
    nuevo_cliente->semaforo = sem_id;
    nuevo_cliente->mem_compartida = mem_compartida;
    memcpy(&nuevo_cliente->info_socket, info_socket_cliente, sizeof(*info_socket_cliente));
    if(pthread_create(&nuevo_cliente->thread, NULL, atender_cliente, (void*)nuevo_cliente) != 0){
        perror("pthread_create");
        free(nuevo_cliente);
        return -1;
    }

    //Agrego el cliente a la lista
    list_add(lista_clientes, nuevo_cliente);
    return 0;
}

/**
 * @brief Elimina de la lista y libera los recursos de los clientes expirados.
 * 
 * @param lista Lista de clientes.
 * @return int Cantidad de clientes destruidos.
 */
int eliminar_clientes_expirados(t_list *lista){
    int cant_inicial;

    bool is_cliente_expirado(cliente_t *cliente){
        return cliente->expiro;
    }

    cant_inicial = list_count_satisfying(lista, (void*)is_cliente_expirado);

    //Por alguna razon esto me tira segmentation fault jeje, lo soluciono con una manera medio crota
    //list_remove_and_destroy_all_by_condition(lista, (void*)is_cliente_expirado, destruir_cliente);
    while(list_count_satisfying(lista, (void*)is_cliente_expirado))
        list_remove_and_destroy_by_condition(lista, (void*)is_cliente_expirado, destruir_cliente);

    return cant_inicial - list_count_satisfying(lista, (void*)is_cliente_expirado);
}

/**
 * @brief Libera todos los recursos del sistema.
 * 
 * @param pid_productor PID del proceso productor de datos.
 * @param config Estructura de configuracion.
 * @param lista_clientes Lista de los clientes abiertos.
 * @param socket_recepcion Socket usado para escuchar pedidos de conexion.
 * @param sem_id Numero del semaforo utilizado.
 * @param shm_id Numero de la memoria compartida utilizada.
 * @param mem_cmp Direccion de la memoria compartida.
 */
void liberar_recursos(pid_t pid_productor, srv_config_t config, t_list *lista_clientes, int socket_recepcion, int sem_id, int shm_id, void *mem_cmp){
    fflush(stdout);

    if(config.config != NULL){
        printf("Liberando configuraciones........");
        destruir_configuracion(config);
        printf("Listo\n");
    }

    if(lista_clientes != NULL){
        printf("Cerrando todos los clientes......");
        list_clean_and_destroy_elements(lista_clientes, destruir_cliente);
        printf("Listo\n");
    }

    if(pid_productor != 0){
        printf("Deteniendo proceso productor.....");
        kill(pid_productor, SIGTERM);     //Le aviso al proceso productor que termine
        waitpid(pid_productor, NULL, 0);
        printf("Listo\n");
    }

    if(socket_recepcion != 0){
        printf("Cerrando el servidor.............");
        cerrar_servidor(socket_recepcion);
        printf("Listo\n");
    }

    if(sem_id != 0){
        printf("Borrando el set de semaforos.....");
        destruir_semaforo(sem_id);
        printf("Listo\n");
    }

    if(shm_id != 0){
        printf("Borrando la memoria compartida...");
        destruir_shmem(shm_id, mem_cmp);
        printf("Listo\n");
    }
}

/**
 * @brief Maneja la recepcion de datos del cliente.
 * Se encarga de analizar los paquetes recibidos y ver si el cliente sigue vivo.
 * 
 * @param datos_cliente Estructura de datos del cliente
 * @return void* 
 */
void* recepcion_tramas_cliente(void *datos_cliente){
    fd_set rfds;
    struct timeval tv;
    int retval;
    cliente_t *self = datos_cliente;
    char buffer[BUFFER_RX_LEN];

    FD_ZERO(&rfds);
    FD_SET(self->socket, &rfds);


    while(salir == 0 && self->terminar == false){
        //Timeout de TIMEOUT_CLIENTE_s segundos
        tv.tv_sec   = TIMEOUT_RX_CLIENTE_s;
        tv.tv_usec  = 0;

        retval = select(self->socket+1, &rfds, NULL, NULL, &tv);

        if (retval == -1){
            perror("select()");
            self->terminar = true;
        } else if (retval){
            /* FD_ISSET(0, &rfds) will be true. */
            if(recv(self->socket, buffer, BUFFER_TX_LEN, 0) <= 0){
                perror("recv");
                self->terminar = 1;
                break;
            }
            
            if(strncmp(MSG_KA, buffer,2)){
                printf("Cerrando cliente por culpa de un '%s'\n", buffer);
                self->terminar = 1;
                break;
            }

        }else{
            printf("Cerrando cliente por timeout\n");
            self->terminar = true;
        }
    }
    pthread_exit(EXIT_SUCCESS);
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
    pthread_t hilo_keep_alive;
    int socket_id, sem_id;
    sensor_datos_t *sensor, datos, dato_crudo, *dato_crudo_compartido;
    sensor_datos_fisicos_t datos_fisicos_crudos, datos_fisicos_filtrados;
    cliente_t *self = (cliente_t*)datos_cliente;
    char buffer_aux[BUFFER_TX_LEN] = {0};
    struct timeval tv;

    socket_id   = self->socket;
    sem_id      = self->semaforo;
    sensor      = &self->mem_compartida->datos_filtrados;
    dato_crudo_compartido = &self->mem_compartida->datos_crudos;

    //Mando inicio de comunicacion
    if(send(socket_id, MSG_INICIO_COMUNICACION, strlen(MSG_INICIO_COMUNICACION),0) == -1){
        perror("send");
        self->terminar = 1;
    }

    //Espero el acknowledge
    if(recv(socket_id, buffer_aux, BUFFER_TX_LEN, 0) <= 0){
        perror("recv");
        self->terminar = 1;
    }
    if(strcmp(buffer_aux, MSG_ACK) == 0){
        if(send(socket_id, MSG_INICIO_COMUNICACION, strlen(MSG_INICIO_COMUNICACION),0) == -1){
            perror("send");
            self->terminar = 1;
        }
    } else
        self->terminar = 1;

    printf("Se conecto un nuevo cliente [Socket: %d]\n", socket_id);

    //Creo un hilo para recibir las tramas del cliente y controlar si sigue vivo
    if(pthread_create(&hilo_keep_alive, NULL, recepcion_tramas_cliente, datos_cliente) != 0){
        perror("pthread_create");
        self->terminar = true;
    }

    while(salir == 0 && self->terminar == false)
    {
        if(control_semaforo_datos(sem_id, SEM_TAKE))
        {
            self->expiro = true;
            kill(self->pid_hilo_principal, SIGUSR1);
            perror("control_semaforo_datos");
            pthread_exit((void*)1);
        }

        memcpy(&datos, sensor, sizeof(datos));
        memcpy(&dato_crudo, dato_crudo_compartido, sizeof(dato_crudo));

        if(control_semaforo_datos(sem_id, SEM_FREE))
        {
            self->expiro = true;
            kill(self->pid_hilo_principal, SIGUSR1);
            perror("control_semaforo_datos");
            pthread_exit((void*)1);
            break;
        }

        // printf("Acel X %d\n", datos.accel.x);
        // printf("Acel Y %d\n", datos.accel.y);
        // printf("Acel Z %d\n", datos.accel.z);
        // printf("Temp %d\n", datos.temp);
        // printf("Gyro X %d\n", datos.gyro.x);
        // printf("Gyro Y %d\n", datos.gyro.y);
        // printf("Gyro Z %d\n", datos.gyro.z);

        convertir_dato_fisico(&datos, &datos_fisicos_filtrados);
        convertir_dato_fisico(&dato_crudo, &datos_fisicos_crudos);

        //memcpy(&datos_fisicos_crudos, &datos_fisicos_filtrados, sizeof(datos_fisicos_crudos));

        sprintf(buffer_aux  , "%.5f\n%.5f\n%.5f\n%.5f\n%.5f\n%.5f\n%.5f\n%.5f\n%.5f\n%.5f\n%.5f\n%.5f\n%.5f\n%.5f\n"
                            , datos_fisicos_crudos.accel.x, datos_fisicos_crudos.accel.y, datos_fisicos_crudos.accel.z
                            , datos_fisicos_crudos.gyro.x, datos_fisicos_crudos.gyro.y, datos_fisicos_crudos.gyro.z
                            , datos_fisicos_crudos.temp
                            , datos_fisicos_filtrados.accel.x, datos_fisicos_filtrados.accel.y, datos_fisicos_filtrados.accel.z
                            , datos_fisicos_filtrados.gyro.x, datos_fisicos_filtrados.gyro.y, datos_fisicos_filtrados.gyro.z
                            , datos_fisicos_filtrados.temp);

        // sprintf(buffer_aux  , "%05d\n%05d\n%05d\n%05d\n%05d\n%05d\n%05d\n"
        //                     , datos.accel.x , datos.accel.y , datos.accel.z
        //                     , datos.gyro.x  , datos.gyro.y  , datos.gyro.z
        //                     , datos.temp);
        // sprintf(buffer_aux   , "%05d\n%05d\n%05d\n%05d\n%05d\n%05d\n%05d\n"
        //     , 0 , 111       , 32767
        //     , -1, -32000    , 2
        //     , 3);
        printf("Enviando %s", buffer_aux);
        
        //Timeout de TIMEOUT_CLIENTE_s segundos
        tv.tv_sec   = TIMEOUT_TX_CLIENTE_s;
        tv.tv_usec  = 0;
        if(setsockopt(self->socket, SOL_SOCKET, SO_SNDTIMEO, (const char*)&tv, sizeof(tv))){
            perror("setsockopt");
            self->terminar = 1;
            break;
        }
        printf("enviando %d bytes\n", strlen(buffer_aux));
        if(send(socket_id, buffer_aux, strlen(buffer_aux), 0) == -1){
            perror("send");
            self->terminar = 1;
            break;
        }
        usleep(100000);
    }

    pthread_join(hilo_keep_alive, NULL);
    self->expiro = true;
    kill(self->pid_hilo_principal, SIGUSR1);
    pthread_exit((void*)0);
}

/**
 * @brief Destruye la estructura "cliente_t"
 * 
 * @param cliente Cliente a destruir
 */
void destruir_cliente(void *cliente)
{
    close(((cliente_t*)cliente)->socket);
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

/**
 * @brief Handler de la señal SIGUSR1.
 * 
 * Le avisa al programa principal que un hilo acaba de terminar.
 * @param sig 
 */
void sigusr1_handler(int sig){
    cliente_expiro = 1;
}

/**
 * @brief Handler de la señal SIGUSR2
 * 
 * Le avisa al programa principal que revise el archivo de configuracion
 * @param sig 
 */
void sigusr2_handler(int sig){
    refrescar_config = 1;
}

/**
 * @brief Lee / define los parametros de configuracion del programa.
 * 
 * @param srv_config Estructura de configuracion.
 * @param path Ruta al archivo de configuracion.
 * @return int 0 existe archivo de configuracion, -1 se cargaron valores por default
 */
int levantar_configuracion(srv_config_t *srv_config, char path[]){
    t_config *config;
    int backlog, cant_conex_maxima, ventana_filtro, puerto;
    int ret = 0;

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
        ret = -1;
    }
    srv_config->backlog             = backlog;
    srv_config->cant_conex_maxima   = cant_conex_maxima;
    srv_config->ventana_filtro      = ventana_filtro;
    srv_config->puerto              = puerto;
    srv_config->config              = config;

    return ret;
}

/**
 * @brief Destruye los recursos utilizados por el manejo del archivo de configuracion.
 * 
 * @param srv_config Estructura de configuracion.
 */
void destruir_configuracion(srv_config_t srv_config){
    config_destroy(srv_config.config);
}
