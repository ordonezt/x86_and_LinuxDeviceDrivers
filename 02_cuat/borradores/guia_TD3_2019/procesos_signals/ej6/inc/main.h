#ifndef __MAIN_H
#define __MAIN_H

#include <sys/types.h>

#define CANTIDAD_HIJOS  10

typedef struct{
    pid_t *p;
    int longitud;
}lista_hijos_t;

int lista_hijos_vacios(lista_hijos_t *lista);
int lista_hijos_agregar(lista_hijos_t *lista, pid_t pid);

#endif /*__MAIN_H*/