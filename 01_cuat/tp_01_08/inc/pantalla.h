#ifndef __PANTALLA_H
#define __PANTALLA_H

#include "../inc/my_types.h"

#define VIDEO_FILAS         25
#define VIDEO_COLUMNAS      80

#define FORMATO_CARACTER_NORMAL     false, color_negro, false, color_blanco
#define FORMATO_CARACTER_BORRADO    false, color_negro, false, color_negro

#define BUFFER_VIDEO ((caracter_video_t *)0x000B8000)

typedef struct{
    uint8_t ascii;
    union{
        uint8_t atributos;
        struct{
            uint8_t color_frontal:3;
            uint8_t negrita:1;
            uint8_t color_trasero:3;
            uint8_t titilando:1;
        };
    };
}caracter_video_t;

typedef enum{
    color_negro     = 0b000,
    color_azul      = 0b001,
    color_verde     = 0b010,
    color_cyan      = 0b011,
    color_rojo      = 0b100,
    color_violeta   = 0b101,
    color_amarillo  = 0b110,
    color_blanco    = 0b111,
}color_caracter_t;

void pantalla_pintar(color_caracter_t color);
void pantalla_putc( uint8_t caracter, uint8_t fila, uint8_t columna, bool parpadear,
                    color_caracter_t color_trasero, bool negrita, color_caracter_t color_frontal);
void my_printf(uint8_t cadena[], uint8_t fila, uint8_t columna);
int16_t fila_columna2indice(uint8_t fila, uint8_t columna);
void hex32_2_str (uint32_t numero, uint8_t *cadena);
void hex64_2_str (uint64_t numero, uint8_t *cadena);
uint32_t strlen(uint8_t cadena[]);
#endif /*__PANTALLA_H*/