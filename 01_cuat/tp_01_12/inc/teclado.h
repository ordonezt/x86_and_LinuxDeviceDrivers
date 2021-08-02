#ifndef __TECLADO_H
#define __TECLADO_H

#include "../inc/my_types.h"

#define LONGITUD_BUFFER_TECLAS  17
#define LONGITUD_TABLA_DIGITOS  6//16

typedef struct{
    uint32_t indice;
    uint64_t promedio;
    union{
        struct{
            uint32_t bajo;
            uint32_t alto;
        };
        uint64_t entero;
    }datos [LONGITUD_TABLA_DIGITOS] __attribute__ ((aligned (8)));
    //uint64_t datos[LONGITUD_TABLA_DIGITOS] __attribute__ ((aligned (8)));
}tabla_digitos_t;

uint8_t teclado_get_tecla(void);
void teclado_inicializar(void);
bool tecla_es_break_code(uint8_t tecla);
bool tecla_es_make_code(uint8_t tecla);
bool tecla_es_enter(uint8_t tecla);
uint8_t tecla2caracter(uint8_t tecla);
bool caracter_es_numero(uint8_t caracter);
void insertar_en_tabla_digitos(uint8_t buffer[], uint32_t longitud);
uint64_t promedio_tabla_digitos(void);
uint64_t suma_tabla_digitos(void);
uint64_t suma_tabla_digitos_saturada_16(void);
uint64_t suma_tabla_digitos_saturada_64(void);
void sumatoria_n_64(uint64_t *vector_datos, uint32_t cantidad, uint64_t *resultado);
uint32_t division_64(uint64_t dividendo, uint32_t divisor, uint64_t *resultado);
#endif /*__TECLADO_H*/
