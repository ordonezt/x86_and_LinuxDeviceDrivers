#include "../../inc/teclado.h"
#include "../../inc/io.h"
#include "../../inc/pic.h"
#include "../../inc/ring_buffer.h"
#include "../../inc/rutinas.h"

#define TECLADO_PUERTO     (0x60)

const uint8_t ascii_LUT[] = {
    // -------- 0 to 9 --------
  ' ',
  ' ', // escape key
  '1','2','3','4','5','6','7','8',
  // -------- 10 to 19 --------
  '9','0','-','=',
  ' ', // Backspace
  ' ', // Tab
  'q','w','e','r',
  // -------- 20 to 29 --------
  't','y','u','i','o','p','[',']',
  '\n', // Enter
  ' ', // left Ctrl
  // -------- 30 to 39 --------
  'a','s','d','f','g','h','j','k','l',';',
  // -------- 40 to 49 --------
  ' ','`',
  ' ', // left Shift
  ' ','z','x','c','v','b','n',
  // -------- 50 to 59 --------
  'm',',','.',
  '/', // slash, or numpad slash if preceded by keycode 224
  ' ', // right Shift
  '*', // numpad asterisk
  ' ', // left Alt
  ' ', // Spacebar
  ' ',
  ' ', // F1
  // -------- 60 to 69 --------
  ' ', // F2
  ' ', // F3
  ' ', // F4
  ' ', // F5
  ' ', // F6
  ' ', // F7
  ' ', // F8
  ' ', // F9
  ' ', // F10
  ' ',
  // -------- 70 to 79 --------
  ' ', // scroll lock
  '7', // numpad 7, HOME key if preceded by keycode 224
  '8', // numpad 8, up arrow if preceded by keycode 224
  '9', // numpad 9, PAGE UP key if preceded by keycode 224
  '-', // numpad hyphen
  '4', // numpad 4, left arrow if preceded by keycode 224
  '5', // numpad 5
  '6', // numpad 6, right arrow if preceded by keycode 224
  ' ',
  '1', // numpad 1, END key if preceded by keycode 224
  // -------- 80 to 89 --------
  '2', // numpad 2, down arrow if preceded by keycode 224
  '3', // numpad 3, PAGE DOWN key if preceded by keycode 224
  '0', // numpad 0, INSERT key if preceded by keycode 224
  '.', // numpad dot, DELETE key if preceded by keycode 224
  ' ',' ',' ',' ',' ',' ',
  // -------- 90 to 99 --------
  ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
  // -------- 100 to 109 --------
  ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
  // -------- 110 to 119 --------
  ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
  // -------- 120-127 --------
  ' ',' ',' ',' ',' ',' ',' ',' ',
};

ring_buffer_t ring_buffer __attribute__ ((section (".datos")));
uint8_t buffer_teclas[LONGITUD_BUFFER_TECLAS] __attribute__ ((section (".datos")));
tabla_digitos_t tabla_digitos __attribute__ ((section (".tabla_digitos")));

__attribute__(( section(".rutinas"))) 
uint8_t teclado_get_tecla(void)
{
    return puerto_io_leer(TECLADO_PUERTO);
}

__attribute__(( section(".rutinas"))) 
void teclado_inicializar(void)
{
    ring_buffer_inicializar(&ring_buffer, buffer_teclas, LONGITUD_BUFFER_TECLAS);

    //En realidad deberia leer las que estan seteadas de antes y jugar con eso
    puerto_io_escribir(PIC1_PUERTO_DATOS, 0xFD);


}

__attribute__(( section(".rutinas"))) 
bool tecla_es_break_code(uint8_t tecla)
{
    return tecla & 0x80;
}

__attribute__(( section(".rutinas"))) 
bool tecla_es_make_code(uint8_t tecla)
{
    return !tecla_es_break_code(tecla);
}

__attribute__(( section(".rutinas"))) 
bool tecla_es_enter(uint8_t tecla)
{
    return tecla & 0x80;
}

__attribute__(( section(".rutinas"))) 
uint8_t tecla2caracter(uint8_t tecla)
{
    if(tecla < 128)
        return ascii_LUT[tecla];
    else
        return 0;
}

__attribute__(( section(".rutinas"))) 
bool caracter_es_numero(uint8_t caracter)
{
    return (caracter >= '0') && (caracter <= '9');
}

__attribute__(( section(".rutinas")))
void insertar_en_tabla_digitos(uint8_t buffer[], uint32_t longitud)
{
    //Cada digito es un nibble
    int8_t i, indice_nibble;
    
    for(i = longitud-1; i >= 0; i--)
    {
        indice_nibble = (longitud - 1 - i);
        if(indice_nibble < 8)
            tabla_digitos.datos[tabla_digitos.indice].bajo |= (buffer[i] - '0') << (indice_nibble * 4);
        else
            tabla_digitos.datos[tabla_digitos.indice].alto |= (buffer[i] - '0') << ((indice_nibble - 8) * 4);
        // asm("xchg %bx,%bx");
        // tabla_digitos.datos[tabla_digitos.indice] |= (buffer[i] - '0') << ((longitud - 1 - i) * 4);
    }
    tabla_digitos.indice++;
}

__attribute__(( section(".std")))
uint64_t promedio_tabla_digitos(void)
{
    uint64_t aux, aux2 = 0;
    uint32_t cant_datos;
    td3_read(&tabla_digitos.indice, &cant_datos, sizeof(cant_datos));

    if(cant_datos != 0)
    {
        //MAGIC_BREAKPOINT
        uint64_t datos[cant_datos];
        td3_read(tabla_digitos.datos, datos, sizeof(datos[0]) * (cant_datos + 1));
        sumatoria_n_64(datos, cant_datos + 1, &aux);
        division_64(aux, cant_datos, &aux2);
    }

    return aux2;
}

__attribute__(( section(".rutinas")))
uint64_t suma_tabla_digitos(void)
{
    uint64_t aux=0;

    if(tabla_digitos.indice != 0)
        sumatoria_n_64((uint64_t *)tabla_digitos.datos, tabla_digitos.indice + 1, &aux);

    return aux;
}

__attribute__(( section(".std")))
uint64_t suma_tabla_digitos_saturada_16(void)
{
    uint64_t aux=0;
    uint32_t cant_datos;
    td3_read(&tabla_digitos.indice, &cant_datos, sizeof(cant_datos));
    uint64_t datos[cant_datos];
    td3_read(tabla_digitos.datos, datos, sizeof(datos[0]) * (cant_datos + 1));

    if(cant_datos != 0)
        //sumatoria_n_64((uint64_t *)tabla_digitos.datos, tabla_digitos.indice + 1, &aux);
        aux = sumatoria_uint16_saturada(datos, cant_datos + 1);

    return aux;
}

__attribute__(( section(".std")))
uint64_t suma_tabla_digitos_saturada_64(void)
{
    uint64_t aux=0;
    uint32_t cant_datos;
    td3_read(&tabla_digitos.indice, &cant_datos, sizeof(cant_datos));
    uint64_t datos[cant_datos];
    td3_read(tabla_digitos.datos, datos, sizeof(datos[0]) * (cant_datos + 1));

    if(cant_datos != 0)
        //sumatoria_n_64((uint64_t *)tabla_digitos.datos, tabla_digitos.indice + 1, &aux);
        aux = sumatoria_uint64_saturada(datos, cant_datos + 1);
    return aux;
}

__attribute__(( section(".std")))
void sumatoria_n_64(uint64_t *vector_datos, uint32_t cantidad, uint64_t *resultado)
{
    uint32_t i;

    *resultado = 0;
    for(i = 0; i < cantidad; i++)   
        *resultado += vector_datos[i];
}

__attribute__(( section(".std")))
uint32_t division_64(uint64_t dividendo, uint32_t divisor, uint64_t *resultado)
{
    uint64_t acumulador = 0, res = 0;
    uint8_t i;

    for(i = 0; i < 64; i++)
    {
        acumulador |= (dividendo >> (63-i)) & 0b1;
        if(acumulador >= divisor)
        {
            res |= 1;
            acumulador -= divisor;
        }
       if(i == 63)
            break;
        res = res << 1;
        acumulador = acumulador << 1;
    }
    
    if(acumulador > divisor)
    {
        res |= 1;
        acumulador -= divisor;
    }
    
    *resultado = res;
    return (uint32_t)acumulador;
}