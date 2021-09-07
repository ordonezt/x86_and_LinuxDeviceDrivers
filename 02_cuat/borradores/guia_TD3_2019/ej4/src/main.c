#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#define LONGITUD_PROMEDIO   10

typedef struct{
    double x,y;
}punto_t;

double promedio(const double datos[], int cantidad);
double varianza(const double datos[], int cantidad);


int main(int argc, char** argv)
{
    FILE *fp;
    //double dato, datos_vec[LONGITUD_PROMEDIO];
    punto_t *puntos_vec, punto_aux;
    int cant_puntos = atoi(argv[2]);

    if(argc == 3){
        if((puntos_vec = (punto_t*)malloc(cant_puntos * sizeof(puntos_vec[0]))) != NULL){

            //printf("\n\nPromedios de %d datos\n", LONGITUD_PROMEDIO);
            printf("Leyendo archivo '%s'...\n\n", argv[1]);
            
            //Abro el archivo en modo lectura
            fp = fopen(argv[1], "r");
            
            //Recorro el archivo y leo las coordenadas
            for(int i = 0; i < cant_puntos; i++){
                if((fscanf(fp, "%lf", &punto_aux.x) != EOF) && (fscanf(fp, "%lf", &punto_aux.y) != EOF))
                    memcpy(&puntos_vec[i], &punto_aux, sizeof(punto_aux));//printf("x: %lf, y: %lf\n", punto_aux.x, punto_aux.y);}
                else
                    break;
            }
            
            printf("Archivo leido!\n");
            //Cierro el archivo
            fclose(fp);

            //Abro el archivo en modo escritura
            fp = fopen("./puntos.dp", "w");
            printf("Guardando puntos en el archivo 'puntos.dp'...\n");
            for(int i = 0; i < cant_puntos; i++){
                fprintf(fp, "x: %lf, y: %lf\n", puntos_vec[i].x, puntos_vec[i].y);
            }
            fclose(fp);
            printf("Listo :)\n");
        }
    }
    else
        printf("Ingrese el path de un archivo y la cantidad de puntos\n");
    
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