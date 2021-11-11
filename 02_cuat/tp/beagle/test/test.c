#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

#define LARGO_DATOS     30

typedef struct{
    short int acc_x;
    short int acc_y;
    short int acc_z;
    short int temp;
    short int gyro_x;
    short int gyro_y;
    short int gyro_z;
}mpu_data;

void formatear_datos(char origen[], mpu_data *destino);
void imprimir_datos(mpu_data dato);

int main(void){
    int file;
    mpu_data datos[LARGO_DATOS];
    char raw_datos[LARGO_DATOS * sizeof(mpu_data)] = {0};

    if((file = open("/dev/TBO_td3_i2c_dev", O_RDWR)) < 0){
        printf("Error abiendo el dispositivo\n");
        return -1;
    }

    // printf("El file es %d\n", file);
    // printf("Voy a leer\n");

    // //write(file, datos, 1);
    // //ioctl(file, 40);

    // read(file, raw_datos, LARGO_DATOS * sizeof(mpu_data));

    // printf("Termine de leer\n");

    // for(int i = 0; i < LARGO_DATOS; i++){
    //     datos[i].acc_x  = (raw_datos[i*sizeof(mpu_data)]        << 8) | raw_datos[i*sizeof(mpu_data)        + 1];
    //     datos[i].acc_y  = (raw_datos[(i*sizeof(mpu_data) + 2)]  << 8) | raw_datos[(i*sizeof(mpu_data) + 2)  + 1];
    //     datos[i].acc_z  = (raw_datos[(i*sizeof(mpu_data) + 4)]  << 8) | raw_datos[(i*sizeof(mpu_data) + 4)  + 1];

    //     datos[i].temp   = (raw_datos[(i*sizeof(mpu_data) + 6)]  << 8) | raw_datos[(i*sizeof(mpu_data) + 6)  + 1];

    //     datos[i].gyro_x = (raw_datos[(i*sizeof(mpu_data) + 8)]  << 8) | raw_datos[(i*sizeof(mpu_data) + 8)  + 1];
    //     datos[i].gyro_y = (raw_datos[(i*sizeof(mpu_data) + 10)] << 8) | raw_datos[(i*sizeof(mpu_data) + 10) + 1];
    //     datos[i].gyro_z = (raw_datos[(i*sizeof(mpu_data) + 12)] << 8) | raw_datos[(i*sizeof(mpu_data) + 12) + 1];
    // }

    // for(int i = 0; i < LARGO_DATOS; i++){
    //     printf("Aceleracion X: %f g\n", (float)datos[i].acc_x / 16384);
    //     printf("Aceleracion Y: %f g\n", (float)datos[i].acc_y / 16384);
    //     printf("Aceleracion Z: %f g\n", (float)datos[i].acc_z / 16384);

    //     printf("Temperatura: %f ºC\n", (float)datos[i].temp / 340 + 36.53);

    //     printf("Giroscopio X: %f º/s\n", (float)datos[i].gyro_x / 131);
    //     printf("Giroscopio Y: %f º/s\n", (float)datos[i].gyro_y / 131);
    //     printf("Giroscopio Z: %f º/s\n", (float)datos[i].gyro_z / 131);
    // }

    for(int i = 0; i < 10000; i++){
        read(file, raw_datos, sizeof(mpu_data));
        formatear_datos(raw_datos, datos);
        imprimir_datos(datos[0]);
        sleep(0.2);
    }

    close(file);
    return 0;
}

void formatear_datos(char origen[], mpu_data *destino){
    destino->acc_x = (origen[0] << 8) | origen[1];
    destino->acc_y = (origen[2] << 8) | origen[3];
    destino->acc_z = (origen[4] << 8) | origen[5];

    destino->temp = (origen[6] << 8) | origen[7];

    destino->gyro_x = (origen[8] << 8) | origen[9];
    destino->gyro_y = (origen[10] << 8) | origen[11];
    destino->gyro_z = (origen[12] << 8) | origen[13];
}

void imprimir_datos(mpu_data dato){
    printf("Aceleracion X: %f g\n", (float)dato.acc_x / 16384);
    printf("Aceleracion Y: %f g\n", (float)dato.acc_y / 16384);
    printf("Aceleracion Z: %f g\n", (float)dato.acc_z / 16384);

    printf("Temperatura: %f ºC\n", (float)dato.temp / 340 + 36.53);

    printf("Giroscopio X: %f º/s\n", (float)dato.gyro_x / 131);
    printf("Giroscopio Y: %f º/s\n", (float)dato.gyro_y / 131);
    printf("Giroscopio Z: %f º/s\n", (float)dato.gyro_z / 131);

    printf("\n\n");
}
