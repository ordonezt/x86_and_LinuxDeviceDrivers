#include "../inc/pantalla.h"

// caracter_video_t buffer_video[VIDEO_FILAS][VIDEO_COLUMNAS] __attribute__ ((section (".buffer_pantalla")));
const uint8_t hex2ascii_LUT[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

__attribute__(( section(".rutinas")))
void pantalla_pintar(color_caracter_t color)
{
    uint16_t fila, columna;
    for(fila = 0; fila < VIDEO_FILAS; fila++)
        for(columna = 0; columna < VIDEO_COLUMNAS; columna++)
            pantalla_putc(0, fila, columna, false, color, false, color);
}

__attribute__(( section(".rutinas")))
void pantalla_putc( uint8_t caracter, uint8_t fila, uint8_t columna, bool parpadear,
                    color_caracter_t color_trasero, bool negrita, color_caracter_t color_frontal)
{
    int16_t indice = fila_columna2indice(fila, columna);

    if(indice != -1)
    {
        BUFFER_VIDEO[indice].ascii = caracter;
        BUFFER_VIDEO[indice].titilando = parpadear;
        BUFFER_VIDEO[indice].color_trasero = color_trasero;
        BUFFER_VIDEO[indice].negrita = negrita;
        BUFFER_VIDEO[indice].color_frontal = color_frontal;
    }
}

__attribute__(( section(".rutinas")))
void my_printf(uint8_t cadena[], uint8_t fila, uint8_t columna)
{
    uint8_t i;

    for(i = 0; cadena[i] != 0; i++)
    {
        pantalla_putc(cadena[i], fila, columna, FORMATO_CARACTER_NORMAL);
        columna++;
        if(columna == VIDEO_COLUMNAS)
        {
            columna = 0;
            fila++;
        }
    }
}

__attribute__(( section(".rutinas")))
int16_t fila_columna2indice(uint8_t fila, uint8_t columna)
{
    if((fila < VIDEO_FILAS) && (columna < VIDEO_COLUMNAS))
        return columna + fila * VIDEO_COLUMNAS;
    else{
        MAGIC_BREAKPOINT
        return -1;
        }
}

__attribute__(( section(".rutinas")))
void hex32_2_str (uint32_t numero, uint8_t *cadena)
{
    uint8_t i=0;
    
    cadena[0]='0';
    cadena[1]='x';
    
    for(i=0;i<8;i++) 
        cadena[1 + 8-i] = hex2ascii_LUT[(numero >> (i*4)) & 0xF];

    cadena[10]=0;
}

__attribute__(( section(".rutinas")))
void hex64_2_str (uint64_t numero, uint8_t *cadena)
{
    uint8_t aux;
    hex32_2_str(numero & 0x00000000FFFFFFFF, cadena+8);
    aux = cadena[10]; //Es pisado por el nulo que pone la funcion que sigue
    hex32_2_str(numero >> 32, cadena);
    cadena[10] = aux;
}

__attribute__(( section(".rutinas")))
uint32_t strlen(uint8_t cadena[])
{
    uint32_t i;

    while(cadena[i++]);

    return i;
}
