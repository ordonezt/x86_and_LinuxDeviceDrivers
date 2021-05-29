#ifndef __TECLADO_H
#define __TECLADO_H

#include "../inc/my_types.h"

#define LONGITUD_BUFFER_TECLAS  17
#define LONGITUD_TABLA_DIGITOS  6//16

typedef struct{
    uint32_t indice;
    struct{
        uint32_t bajo;
        uint32_t alto;
    }datos [LONGITUD_TABLA_DIGITOS] __attribute__ ((aligned (8)));
}tabla_digitos_t;

uint8_t teclado_get_tecla(void);
void teclado_inicializar(void);
bool tecla_es_break_code(uint8_t tecla);
bool tecla_es_make_code(uint8_t tecla);
bool tecla_es_enter(uint8_t tecla);
uint8_t tecla2caracter(uint8_t tecla);
bool caracter_es_numero(uint8_t caracter);
void insertar_en_tabla_digitos(uint8_t buffer[], uint32_t longitud);

#endif /*__TECLADO_H*/
