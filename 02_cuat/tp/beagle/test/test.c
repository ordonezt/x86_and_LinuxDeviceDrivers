#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(void){
    int file;

    if(file = open("/dev/TBO_td3_i2c_dev", O_RDWR) < 0){
        printf("Error abiendo el dispositivo\n");
        return -1;
    }
    close(file);
    return 0;
}
