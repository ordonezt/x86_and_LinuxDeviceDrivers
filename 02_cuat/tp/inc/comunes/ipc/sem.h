#ifndef __SEM_H
#define __SEM_H

#include <sys/sem.h>

#define SEM_TAKE -1						
#define SEM_FREE 1

/**
 * \fn int crear_semaforo (key_t *LLAVE, int nsems, int ATRIBUTOS, int _8bits)
 * \brief Funcion que crea e inicializa uno o varios semaforos.
 * \details Esta funcion se encarga de crear e inicializar uno o varios semaforos (segun se asigne a nsems)
		 Verifica que no exista ya un semaforo, con esa key_t y se lo intenta reutilziar si se puede.
 * \param *LLAVE es un puntero a la variable que almacena el id del semaforo, dentro del programa que invoca
		a esta funcion.
 * \param nsems es el numero de semaforos que contendra el set a crear.
 * \param ATRIBUTOS son los atributos del mismo. 
 * \param _8bits parametros para completar el proceso de creacion de la llave.
 * \return int ante un exito, retorna el id del semaforo creado. Retorna -1 si no hubo exito con ERRNO
		indicando el error.
 * \author	Luciano Ferreyro
**/

int crear_semaforo (int nsems, key_t llave);

/**
 * \fn int control_semaforo (int ID, int N_SEM)
 * \brief Funcion que bloquea o libera un semaforo de un determinado set.
 * \details Esta funcion se encarga de bloquear o liberar a un semaforo dentro de un determinado set de 
		 semaforos. Esta funcion es validad unicamente para 1 solo set de semaforos, y no para un array de 
		 struct sembuf - aunque esta funcion es facilmente modificable para cumplir con eso.
 * \param ID es el identificador del semaforo o set de semaforos que se desea bloquear.
 * \param N_SEM es el numero de semaforo que se quiere bloquear, dentro de un set de semaforos.
 * \param ACCION puede ser SEM_TAKE o SEM_FREE, segun se desee.
 * \return int Retorna -1 si no hubo exito, y retorna 0 si lo hubo
 * \author	Luciano Ferreyro
**/
int control_semaforo (int ID, int N_SEM, int ACCION);

/**
 * @brief Destruye un set de semaforos
 * 
 * @param sem_id Id del set de semaforo a destruir
 * @return int 0 exito, -1 error
 */
int destruir_semaforo(int sem_id);

#endif /*__SEM_H*/
