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
#include "../../inc/sensor/dsp.h"
#include "../../inc/comunes/ipc/sem.h"
#include "../../inc/comunes/ipc/sockets.h"
#include "../../inc/comunes/ipc/sharmem.h"
#include "../../inc/servidor/servidor.h"
#include "../../inc/comunes/ipc/signal.h"
#include "../../inc/comunes/collections/list.h"

void sigusr2_handler(int sig);
void sigint_handler(int sig);
int procesar_dato(t_list *lista_estados, sensor_datos_t *dato_nuevo, sensor_datos_t *dato_procesado, int ventana_filtro);
int media_q15(int16_t *resultado, int16_t datos[], int32_t longitud);
int llenar_vector_datos_q15(t_list *lista, int16_t vector[], int longitud, int offset);
void list_destruir_estado(sensor_datos_t *dato);
int inicializar_recursos( int argc, char* argv[], int *sem_id, int *ventana_filtro, pid_t *pid_servidor, int *shm_id
                        , sensor_t *sensor, char sensor_path[], t_list **lista_estados, datos_compartidos_t **mem_compartida);
void liberar_recursos(pid_t pid_servidor, sensor_t *sensor, t_list *lista_estados);

volatile sig_atomic_t salir, flag_cambio_config;

/**
 * @brief Programa que maneja el acceso al sensor
 * 
 * @return int 
 */
int main(int argc, char* argv[])
{
    sensor_t sensor;
    int sem_id, shm_id;
    datos_compartidos_t *mem_compartida;
    t_list *lista_estados;
    int ventana_filtro;
    sensor_datos_t dato_filtrado;
    pid_t pid_servidor;


    if(inicializar_recursos(argc, argv, &sem_id, &ventana_filtro, &pid_servidor, &shm_id, &sensor, SENSOR_PATH, &lista_estados, &mem_compartida) == -1){
        perror("inicializar_recursos");
        exit(1);
    }

    while(salir == false)
    {

        if(flag_cambio_config){
            flag_cambio_config = 0;
            ventana_filtro = mem_compartida->ventana_filtro;
            printf("Nueva ventana %d\n", mem_compartida->ventana_filtro);
        }

        //Leer sensor
        if(leer_sensor(&sensor, &sensor.datos) <= 0)
        {
            perror("Error en la lectura del sensor\n");
            break;
        }
        else
        {
            //Filtro el dato nuevo
            procesar_dato(lista_estados, &sensor.datos, &dato_filtrado, ventana_filtro);

            //Pido el semaforo
            if(control_semaforo(sem_id, N_SEMAFORO_DATOS, SEM_TAKE))
            {
                perror("control_semaforo");
                break;
            }

            //Guardo los datos en la memoria compartida
            memcpy(&mem_compartida->datos_filtrados, &dato_filtrado, sizeof(sensor.datos));
            memcpy(&mem_compartida->datos_crudos, &sensor.datos, sizeof(sensor.datos));
            
            //Libero el semaforo
            if(control_semaforo(sem_id, N_SEMAFORO_DATOS, SEM_FREE))
            {
                perror("control_semaforo");
                break;
            }

            usleep(10000);
        }
    }

    liberar_recursos(pid_servidor, &sensor, lista_estados);

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
    char buffer[sizeof(sensor_datos_t)];
    int bytes_leidos;

    bytes_leidos = read(sensor->fd, buffer, sizeof(sensor_datos_t));

    if(bytes_leidos != sizeof(sensor_datos_t))
        return -1;
    
    datos->accel.x  = (buffer[0] << 8)  | buffer[1];
    datos->accel.y  = (buffer[2] << 8)  | buffer[3];
    datos->accel.z  = (buffer[4] << 8)  | buffer[5];
    datos->temp     = (buffer[6] << 8)  | buffer[7];
    datos->gyro.x   = (buffer[8] << 8)  | buffer[9];
    datos->gyro.y   = (buffer[10] << 8) | buffer[11];
    datos->gyro.z   = (buffer[12] << 8) | buffer[13];

    return bytes_leidos;
}

/**
 * @brief Destruye un elemento de una lista de datos de sensor
 * 
 * @param dato Puntero a dato a liberar
 */
void list_destruir_estado(sensor_datos_t *dato){
    free(dato);
}

/**
 * @brief Procesa un nuevo dato de sensor.
 * Agrega, remueve y desplaza datos en la lista de estados segun la cantidad de promedios a aplicar.
 * Una vez organizada la lista realiza el promedio.
 * 
 * @param lista_estados Lista de estados anteriores del sensor.
 * @param dato_nuevo Puntero a nuevos datos.
 * @param dato_procesado Donde se va a guardar el dato procesado.
 * @param ventana_filtro Cantidad de promedios a aplicar.
 * @return int 0 exito, -1 error.
 */
int procesar_dato(t_list *lista_estados, sensor_datos_t *dato_nuevo, sensor_datos_t *dato_procesado, int ventana_filtro){
    sensor_datos_t *dato_aux;
    int16_t *vector_aux;
    int diferencia_ventana;
    int largo_lista = list_size(lista_estados);

    if(largo_lista == ventana_filtro)
        list_remove_and_destroy_element(lista_estados, largo_lista - 1, (void*)list_destruir_estado);
    else if(largo_lista > ventana_filtro){
        diferencia_ventana = largo_lista - ventana_filtro + 1; //El +1 es para remover el ultimo dato, porque tiene que entrar el nuevo
        while(diferencia_ventana--)
            list_remove_and_destroy_element(lista_estados, ventana_filtro - 1 + diferencia_ventana, (void*)list_destruir_estado);
    }
    
    dato_aux = (sensor_datos_t *)malloc(sizeof(dato_aux[0]));
    memcpy(dato_aux, dato_nuevo, sizeof(dato_aux[0]));
    list_add_in_index(lista_estados, 0, dato_aux);
    /*
    Genero vectores con los datos a filtrar de cada campo con el objetivo de facilitar la funcion de promedio
    que tiene que ser compilada con optimizacion y despues analizada.
    De esta manera me soluciono un par de bolonquis.
    */
    largo_lista = list_size(lista_estados);
    vector_aux = malloc(sizeof(*vector_aux) * largo_lista);
    if(vector_aux == NULL){
        perror("malloc");
        return -1;
    }

    llenar_vector_datos_q15(lista_estados, vector_aux, largo_lista, 0); //Aceleracion x
    media_q15(&dato_procesado->accel.x, vector_aux, largo_lista);

    llenar_vector_datos_q15(lista_estados, vector_aux, largo_lista, 2); //Aceleracion y
    media_q15(&dato_procesado->accel.y, vector_aux, largo_lista);

    llenar_vector_datos_q15(lista_estados, vector_aux, largo_lista, 4); //Aceleracion z
    media_q15(&dato_procesado->accel.z, vector_aux, largo_lista);

    llenar_vector_datos_q15(lista_estados, vector_aux, largo_lista, 6); //Temp
    media_q15(&dato_procesado->temp, vector_aux, largo_lista);

    llenar_vector_datos_q15(lista_estados, vector_aux, largo_lista, 8); //Gyro x
    media_q15(&dato_procesado->gyro.x, vector_aux, largo_lista);

    llenar_vector_datos_q15(lista_estados, vector_aux, largo_lista, 10); //Gyro y
    media_q15(&dato_procesado->gyro.y, vector_aux, largo_lista);

    llenar_vector_datos_q15(lista_estados, vector_aux, largo_lista, 12); //Gyro z
    media_q15(&dato_procesado->gyro.z, vector_aux, largo_lista);
    
    free(vector_aux);
    return 0;
}

int inicializar_recursos(int argc, char* argv[], int *sem_id, int *ventana_filtro, pid_t *pid_servidor, int *shm_id, sensor_t *sensor, char sensor_path[], t_list **lista_estados, datos_compartidos_t **mem_compartida){
    key_t llave;
    signal_t sig_int, sig_usr2, sig_term;

    if(argc < 3){
        printf("Ingrese los argumentos necesarios\n");
        return -1;
    }

    //Leo el set de semaforo de los argumentos
    *sem_id = atoi(argv[1]);

    *ventana_filtro = atoi(argv[2]);

    *pid_servidor = atoi(argv[3]);

    //Creo la llave para los ipc
    llave = ftok(CONFIG_PATH, 'T');
    if(llave < 0){
        perror("Creación de llave");
        return -1;
    }

    //Manejo de la señal sigint (Le avisa al programa principal que hay que cerrar todo)
    salir = 0;
    if(atrapar_signal(&sig_int, sigint_handler, SIGINT)){
        perror("atrapar_signal");
        return -1;
    }

    //Manejo de la señal sigterm (Le avisa al programa principal que hay que cerrar todo)
    salir = 0;
    if(atrapar_signal(&sig_term, sigint_handler, SIGTERM)){
        perror("atrapar_signal");
        return -1;
    }
    
    //Manejo de la señal sigusr2 (Avisa que hay que refrescar las configuraciones)
    flag_cambio_config = 0;
    if(atrapar_signal(&sig_usr2, sigusr2_handler, SIGUSR2)){
        perror("atrapar_signal");
        return -1;
    }

    //Abro el sensor
    if(abrir_sensor(sensor, sensor_path) == -1)
    {
        perror("No se pudo abrir el sensor\n");
        return -1;
    }

    //Creo la memoria compartida
    *shm_id = crear_shmem((void**)mem_compartida, llave, sizeof((*mem_compartida)[0]));
    if(shm_id < 0){
        perror("crear_shmem");
        liberar_recursos(*pid_servidor, sensor, NULL);
        return -1;
    }
    
    //Creo una lista para guardar los estados anteriores del sensor 
    //(y poder aplicar un filtro FIR)
    *lista_estados = list_create();
    if(lista_estados == NULL){
        perror("list_create");
        liberar_recursos(*pid_servidor, sensor, NULL);
        return -1;
    }

    //Libero el semaforo
    if(control_semaforo(*sem_id, N_SEMAFORO_DATOS, SEM_FREE))
    {
        perror("control_semaforo");
        liberar_recursos(*pid_servidor, sensor, *lista_estados);
        return -1;
    }

    return 0;
}

/**
 * @brief Libera los recursos utilizados
 * 
 * @param sensor Puntero al sensor
 * @param lista_estados Lista de estados del filtro
 */
void liberar_recursos(pid_t pid_servidor, sensor_t *sensor, t_list *lista_estados){

    kill(pid_servidor, SIGTERM);     //Le aviso al proceso principal que termine

    printf("\nCerrando el sensor...............");
    if(cerrar_sensor(sensor) == -1)
    {
        perror("\nError cerrando el sensor\n");
        exit(1);
    }
    else
        printf("Listo\n");

    if(lista_estados != NULL){
        printf("Removiendo filtro................");
        list_destroy_and_destroy_elements(lista_estados, (void*)list_destruir_estado);
        printf("Listo\n");
    }
}

/**
 * @brief Compone un array de datos "continuos" a partir de una lista.
 * 
 * @param lista Lista de datos
 * @param vector Vector donde se vuelvan los datos
 * @param longitud Longitud del vector a llenar
 * @param offset Offset en bytes del dato dentro de la estructura del elemento de la lista.
 * @return int 0 exito, -1 error
 */
int llenar_vector_datos_q15(t_list *lista, int16_t vector[], int longitud, int offset){
    char *p;

    for(int i = 0; i < longitud; i++){
        p = list_get(lista, i);
        vector[i] = *((int16_t*)(p + offset));
    }
    return 0;
}

/**
 * @brief Handler de la señal USR2
 * 
 * @param sig 
 */
void sigusr2_handler(int sig){
    flag_cambio_config = 1;
}

/**
 * @brief Handler de la señal SIGINT.
 * 
 * Le avisa al programa principal que hay que cerrar todo.
 * 
 * @param sig 
 */
void sigint_handler(int sig){
    salir = 1;
}
