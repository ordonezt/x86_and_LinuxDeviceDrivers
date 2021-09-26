#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#define LONGITUD_PROMEDIO   10

double promedio(const double datos[], int cantidad);
double varianza(const double datos[], int cantidad);

int main(int argc, char** argv)
{
    FILE *fp;
    double dato, datos_vec[LONGITUD_PROMEDIO];
    
    if(argc == 2){
        printf("\n\nPromedios de %d datos\n", LONGITUD_PROMEDIO);
        printf("Leyendo archivo '%s'...\n\n", argv[1]);
        
        //Abro el archivo en modo lectura
        fp = fopen(argv[1], "r");
        
        for(int i=0; fscanf(fp, "%lf", &dato) != EOF; i++){
            datos_vec[i] = dato;
            if(i == LONGITUD_PROMEDIO-1){
                i = -1;
                printf("Promedio: %lf\n", promedio(datos_vec, LONGITUD_PROMEDIO));
                printf("Varianza: %lf\n", varianza(datos_vec, LONGITUD_PROMEDIO));
            }
        }
        
        printf("Archivo leido!\n");

        //Cierro el archivo
        fclose(fp);
    }
    else
        write(0, "Ingrese el path de un archivo", strlen("Ingrese el path de un archivo"));
    
    return 0;
}

double promedio(const double datos[], int cantidad){
    double acumulador = 0;
    for(int i=0; i < cantidad; i++)
        acumulador += datos[i];
    return acumulador / cantidad;
}

double varianza(const double datos[], int cantidad){
    double acumulador = 0, prom = promedio(datos, cantidad);
    for(int i=0; i < cantidad; i++)
        acumulador += (datos[i] - prom);
    return acumulador / cantidad;
}