#include "../../inc/pantalla.h"
#include "../../inc/rutinas.h"

const uint8_t hex2ascii_LUT[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

//Pinta toda la pantalla de un color
__attribute__(( section(".rutinas")))
void pantalla_pintar(color_caracter_t color)
{
    uint16_t fila, columna;
    for(fila = 0; fila < VIDEO_FILAS; fila++)
        for(columna = 0; columna < VIDEO_COLUMNAS; columna++)
            pantalla_putc(0, fila, columna, false, color, false, color);
}

//Escribe un caracter en pantalla
__attribute__(( section(".rutinas")))
int8_t pantalla_putc( uint8_t caracter, uint8_t fila, uint8_t columna, bool parpadear,
                    color_caracter_t color_trasero, bool negrita, color_caracter_t color_frontal)
{
    int8_t retorno = 0;
    int16_t indice = fila_columna2indice(fila, columna);

    if(indice != -1)
    {
        BUFFER_VIDEO[indice].ascii = caracter;
        BUFFER_VIDEO[indice].titilando = parpadear;
        BUFFER_VIDEO[indice].color_trasero = color_trasero;
        BUFFER_VIDEO[indice].negrita = negrita;
        BUFFER_VIDEO[indice].color_frontal = color_frontal;
    }
    else
        retorno = -1;
    
    return retorno;
}

//Escribe una cadena de caracteres en pantalla
__attribute__(( section(".rutinas")))
int8_t my_printf(uint8_t cadena[], uint8_t fila, uint8_t columna)
{
    int8_t retorno = 0;
    uint8_t i;

    for(i = 0; cadena[i] != 0; i++)
    {
        if(pantalla_putc(cadena[i], fila, columna, FORMATO_CARACTER_NORMAL) == 0)
        {
            columna++;
            if(columna == VIDEO_COLUMNAS)
            {
                columna = 0;
                fila++;
            }
        }
        else
            retorno = -1;
    }
    return retorno;
}

//Convierte la fila y columnas en indice de memoria de video
__attribute__(( section(".rutinas")))
int16_t fila_columna2indice(uint8_t fila, uint8_t columna)
{
    if((fila < VIDEO_FILAS) && (columna < VIDEO_COLUMNAS))
        return columna + fila * VIDEO_COLUMNAS;
    else
        return -1;
}

//Convierte un numero de 32 bits en una cadena de caracteres
__attribute__(( section(".std")))
void hex32_2_str (uint32_t numero, uint8_t *cadena)
{
    
    uint8_t i=0, aux;

    cadena[0]='0';
    cadena[1]='x';
    
    //Esta es la forma mas elegante, pero por el tema de protecciones no me deja acceder a la tabla
    // for(i=0;i<8;i++) 
    //     cadena[1 + 8-i] = hex2ascii_LUT[(numero >> (i*4)) & 0xF];
    
    for(i=0;i<8;i++)
    {
        aux = (numero >> (i*4)) & 0xF;
        if(aux == 0)
            cadena[1 + 8-i] = '0';
        else if(aux == 1)
            cadena[1 + 8-i] = '1';
        else if(aux == 2)
            cadena[1 + 8-i] = '2';
        else if(aux == 3)
            cadena[1 + 8-i] = '3';
        else if(aux == 4)
            cadena[1 + 8-i] = '4';
        else if(aux == 5)
            cadena[1 + 8-i] = '5';
        else if(aux == 6)
            cadena[1 + 8-i] = '6';
        else if(aux == 7)
            cadena[1 + 8-i] = '7';
        else if(aux == 8)
            cadena[1 + 8-i] = '8';
        else if(aux == 9)
            cadena[1 + 8-i] = '9';
        else if(aux == 10)
            cadena[1 + 8-i] = 'A';
        else if(aux == 11)
            cadena[1 + 8-i] = 'B';
        else if(aux == 12)
            cadena[1 + 8-i] = 'C';
        else if(aux == 13)
            cadena[1 + 8-i] = 'D';
        else if(aux == 14)
            cadena[1 + 8-i] = 'E';
        else if(aux == 15)
            cadena[1 + 8-i] = 'F';
    }
    cadena[10]=0;
}

//Convierte un numero de 64 bits en una cadena de caracteres
__attribute__(( section(".std")))
void hex64_2_str (uint64_t numero, uint8_t *cadena)
{
    uint8_t aux;
    hex32_2_str(numero & 0x00000000FFFFFFFF, cadena+8);
    aux = cadena[10]; //Es pisado por el nulo que pone la funcion que sigue
    hex32_2_str(numero >> 32, cadena);
    cadena[10] = aux;
}

//Devuelve la longitud de una cadena de caracteres
__attribute__(( section(".std")))
uint32_t strlen(uint8_t cadena[])
{
    uint32_t i;

    while(cadena[i++]);

    return i;
}
