#ifndef _TBO_TD3_I2C_DEV_H
#define _TBO_TD3_I2C_DEV_H

#define PRIMER_MENOR    0
#define CANT_DISP       1
#define CDEV_NAME       "td3_cdev"
#define COMPATIBLE      "TBO_td3_i2c_dev"
#define CLASS_NAME      "TBO_td3_i2c_class"

typedef enum{
    SRE_ESPERAR,
    SRE_LEER
}sensor_read_estados;

#endif /*_TBO_TD3_I2C_DEV_H*/
