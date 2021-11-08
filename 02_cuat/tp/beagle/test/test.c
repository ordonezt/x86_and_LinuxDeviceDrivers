#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define LARGO_DATOS     10

typedef struct{
    short int acc_x;
    short int acc_y;
    short int acc_z;
    short int temp;
    short int gyro_x;
    short int gyro_y;
    short int gyro_z;
}mpu_data;

int main(void){
    int file;
    mpu_data datos[LARGO_DATOS];

    if(file = open("/dev/TBO_td3_i2c_dev", O_RDWR) < 0){
        printf("Error abiendo el dispositivo\n");
        return -1;
    }

    printf("Voy a leer\n");

    //write(file, datos, 1);
    //ioctl(file, 40);

    read(file, datos, 1);//LARGO_DATOS * sizeof(mpu_data));

    for(int i = 0; i < LARGO_DATOS; i++){
        printf("Aceleracion X: %d\n", datos[i].acc_x);
        printf("Aceleracion Y: %d\n", datos[i].acc_y);
        printf("Aceleracion Z: %d\n", datos[i].acc_z);

        printf("Temperatura: %d\n", datos[i].temp);

        printf("Giroscopio X: %d\n", datos[i].gyro_x);
        printf("Giroscopio Y: %d\n", datos[i].gyro_y);
        printf("Giroscopio Z: %d\n", datos[i].gyro_z);
    }

    close(file);
    return 0;
}
