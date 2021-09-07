#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

int main(int argc, char** argv)
{
    FILE *fp;
    struct stat st;
    char* contenido;

    //Obtengo el estado del archivo (el nombre esta en la macro __FILE__)
    stat(__FILE__, &st);

    //Creo un buffer con el tamaño acorde para almacenar el contenido
    contenido = (char*)malloc(st.st_size);

    write(0, "\n\nEl contenido del archivo es: \n\n", strlen("\n\nEl contenido del archivo es: \n\n"));
    
    //Abro el archivo en modo lectura
    fp = fopen(__FILE__, "r");
    
    //Leo el archivo y verifique haer leido el tamaño correcto
    if(fread(contenido, sizeof(contenido[0]), st.st_size, fp) != (st.st_size / sizeof(contenido[0])))
        write(0, "Error de lectura", strlen("Error de lectura"));
    else
        //Imprimo el contenido en pantalla
        write(0, contenido, st.st_size * sizeof(contenido[0]));
    
    write(0, "\n\n", 2);

    //Libero el buffer de contenido
    free(contenido);

    //Cierro el archivo
    fclose(fp);
    
    return 0;
}