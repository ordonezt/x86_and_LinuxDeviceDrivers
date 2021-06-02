#include "../inc/ring_buffer.h"

__attribute__(( section(".rutinas")))
uint8_t ring_buffer_insertar(ring_buffer_t *ring_buffer, uint8_t dato)
{
    if(ring_buffer_get_libre(ring_buffer))
    {
        //Pongo
        ring_buffer->buffer[ring_buffer->cabeza++] = dato;
        //Aumento
        if(ring_buffer->cabeza == ring_buffer->longitud)
            ring_buffer->cabeza = 0;
        return 1;
    }
    return 0;
}

__attribute__(( section(".rutinas")))
uint8_t ring_buffer_pop(ring_buffer_t *ring_buffer, uint8_t *dato)
{
    if(ring_buffer_get_ocupado(ring_buffer))
    {
        //Saco
        *dato = ring_buffer->buffer[ring_buffer->cola++];
        //Aumento
        if(ring_buffer->cola == ring_buffer->longitud)
            ring_buffer->cola = 0;
        return 1;
    }
    return 0;
}

__attribute__(( section(".rutinas")))
uint32_t ring_buffer_get_libre(ring_buffer_t *ring_buffer)
{
   if(ring_buffer->cabeza > ring_buffer->cola)
        return ring_buffer->longitud - ring_buffer->cabeza + ring_buffer->cola - 1;
    else
        return ring_buffer->cola - ring_buffer->cabeza -1;
}

__attribute__(( section(".rutinas")))
uint32_t ring_buffer_get_ocupado(ring_buffer_t *ring_buffer)
{
    return ring_buffer->longitud - 1 - ring_buffer_get_libre(ring_buffer);
}

__attribute__(( section(".rutinas")))
bool ring_buffer_esta_lleno(ring_buffer_t *ring_buffer)
{
    return ring_buffer_get_libre(ring_buffer) == 0;
}

__attribute__(( section(".rutinas")))
bool ring_buffer_esta_vacio(ring_buffer_t *ring_buffer)
{
    return ring_buffer_get_libre(ring_buffer) == ring_buffer->longitud;
}

__attribute__(( section(".rutinas")))
void ring_buffer_inicializar(ring_buffer_t *ring_buffer, uint8_t *buffer, uint32_t longitud)
{
    ring_buffer->buffer = buffer;
    ring_buffer->longitud = longitud;
    ring_buffer->cabeza = 0;
    ring_buffer->cabeza = 0;
}

__attribute__(( section(".rutinas")))
void ring_buffer_vaciar(ring_buffer_t *ring_buffer)
{
    ring_buffer->cabeza = 0;
    ring_buffer->cabeza = 0;
}