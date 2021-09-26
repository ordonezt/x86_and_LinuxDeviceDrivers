#ifndef __MAIN_H
#define __MAIN_H

#include <sys/types.h>

typedef struct{
    pid_t *p;
    int longitud;
}lista_hijos_t;

int lista_hijos_vacios(lista_hijos_t *lista);
int lista_hijos_agregar(lista_hijos_t *lista, pid_t pid);
int esperar_hijos(lista_hijos_t *lista);

#endif /*__MAIN_H*/