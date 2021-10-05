#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <fcntl.h>
#include "../../../inc/comunes/ipc/datos.h"

int crear_shmem(void **memoria, key_t llave, int longitud);
void destruir_shmem (int shm_id, void *mem);
void* vincular_shmem(int shm_id);

/**
 * \fn void crear_shmem (key_t *LLAVE, int *ID, int TAM, char *ATTACH, int _8bits, int PERMISOS)
 * \brief Funcion que crear una shared memory.
 * \details Esta funcion se encarga de reservar, crear y asignar el espacio de memoria compartida.
 * \param *LLAVE puntero a una variable que contendra la key para identificar el proceso. 
 * \param *ID puntero a la variable que contendra el identificador al espacio de memoria reservado/memoria compartida.
 * \param TAM es el tamaño del espacio de memoria reservado.
 * \param *ATTACH es un puntero, ya que representa al dato que "attacheamos" al espacio reservado.
 * \param _8bits representa el valor que debemos agregar en ftok y que utiliza dicha funcion para generar
	   la clave. Se lo otorga como variable, ya que variando ese parametro generamos claves diferentes
	   para un mismo archivo .c.
 * \param PERMISOS son los permisos que poseera el espacio de memoria reservado.
 * \return void 
 * \author	Luciano Ferreyro
**/	

int crear_shmem(void **memoria, key_t llave, int longitud)
{
	int shm_id;
	
	/*Creo la shared memory para los procesos. Obtengo el identificador de la posicion de memoria compartida para cada proceso*/	
	if((shm_id = shmget(llave , longitud, 0644 | IPC_CREAT)) == -1)
	{
		perror("shmget");
		return -1;
	}
	
	/*Ahora attacheamos ese espacio compartido a un dato*/
	(*memoria) = shmat(shm_id, NULL, 0);
	if ((*memoria) == (void*)(-1))
	{
		perror("shmat");
		return -1;
	}	
	
	return shm_id;
}

void* vincular_shmem(int shm_id){
	/*Ahora attacheamos ese espacio compartido a un dato*/
	void *memoria;

	memoria = shmat(shm_id, NULL, 0);
	if (memoria == (void*)(-1))
	{
		perror("shmat");
		return NULL;
	}
	return memoria;
}

/**
 * \fn void destruir_shmem (int ID, char *ATTACH)
 * \brief Funcion que destruye una shared memory.
 * \details Esta funcion se encarga de deligar (detach) de un segmento de memoria compartida y destruir dicho
		 segmento (eliminarlo del sistema).
 * \param ID identificador del segmento de memoria a eliminar.
 * \param *ATTACH es un puntero, ya que representa al dato que desligaremos de la memoria compartida.
 * \return void 
 * \author	Luciano Ferreyro
**/

void destruir_shmem (int shm_id, void *mem)
{
	/*Primero desligo ATTACH del segmento*/	
	if(shmdt(mem) == -1)
	{
		if(errno!=EINVAL)
		    perror("shmdt");
	//	exit(1);
	}
	/*Elimino el segmento*/
	if(shmctl(shm_id,IPC_RMID,NULL) == -1)
	{
		if(errno!=EINVAL)
			perror("shmctl");
	//	exit(1);
	}
}
