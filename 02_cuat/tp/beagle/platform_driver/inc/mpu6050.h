#ifndef _MPU6050_H
#define _MPU6050_H

#include <linux/kernel.h>

#define MPU6050_ADDRESS             0x068
#define MPU6050_RA_USER_CTRL        0x06A
#define MPU6050_RA_PWR_MGMT_1       0x06B       
#define MPU6050_RA_CONFIG           0x01A
#define MPU6050_RA_SMPLRT_DIV       0x019       
#define MPU6050_RA_GYRO_CONFIG      0x01B      
#define MPU6050_RA_ACCEL_CONFIG     0x01C     
#define MPU6050_RA_FIFO_EN          0x023
#define MPU6050_RA_FIFO_COUNTH      0x072
#define MPU6050_RA_FIFO_COUNTL      0x073
#define MPU6050_RA_INT_PIN_CFG      0x037      
#define MPU6050_RA_INT_ENABLE       0x038       
#define MPU6050_RA_FIFO_R_W         0x074

#define MPU6050_RA_ACCEL_XOUT_H     0x03B
#define MPU6050_RA_ACCEL_XOUT_L     0x03C
#define MPU6050_RA_ACCEL_YOUT_H     0x03D
#define MPU6050_RA_ACCEL_YOUT_L     0x03E
#define MPU6050_RA_ACCEL_ZOUT_H     0x03F
#define MPU6050_RA_ACCEL_ZOUT_L     0x040
#define MPU6050_RA_TEMP_OUT_H       0x041
#define MPU6050_RA_TEMP_OUT_L       0x042
#define MPU6050_RA_GYRO_XOUT_H      0x043
#define MPU6050_RA_GYRO_XOUT_L      0x044
#define MPU6050_RA_GYRO_YOUT_H      0x045
#define MPU6050_RA_GYRO_YOUT_L      0x046
#define MPU6050_RA_GYRO_ZOUT_H      0x047
#define MPU6050_RA_GYRO_ZOUT_L      0x048

#define MPU6050_RA_WHO_AM_I         0x75

int sensor_open(struct inode *node, struct file *f);
int sensor_release(struct inode *node, struct file *f);
ssize_t sensor_read(struct file *flip, char __user *buf, size_t count, loff_t *pos);
ssize_t sensor_write (struct file *flip, const char __user *buf, size_t count, loff_t *pos);
long sensor_ctrl(struct file *flip, unsigned int cmd, unsigned long values);

#endif /*_MPU6050_H*/
