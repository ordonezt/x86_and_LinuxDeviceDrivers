#ifndef __RING_BUFFER_H
#define __RING_BUFFER_H

#include "../inc/my_types.h"

typedef struct{
    uint32_t cabeza;    //Indice de la proxima posicion a escribir
    uint32_t cola;      //Indice de la posicion a sacar
    uint32_t longitud;
    uint8_t *buffer;
}ring_buffer_t;

uint8_t ring_buffer_insertar(ring_buffer_t *ring_buffer, uint8_t dato);
uint8_t ring_buffer_pop(ring_buffer_t *ring_buffer, uint8_t *dato);
uint32_t ring_buffer_get_libre(ring_buffer_t *ring_buffer);
uint32_t ring_buffer_get_ocupado(ring_buffer_t *ring_buffer);
bool ring_buffer_esta_lleno(ring_buffer_t *ring_buffer);
bool ring_buffer_esta_vacio(ring_buffer_t *ring_buffer);
void ring_buffer_inicializar(ring_buffer_t *ring_buffer, uint8_t *buffer, uint32_t longitud);
void ring_buffer_vaciar(ring_buffer_t *ring_buffer);

#endif /*__RING_BUFFER_H*/