#ifndef _MPU6050_H
#define _MPU6050_H

#include <linux/kernel.h>

int sensor_open(struct inode *node, struct file *f);
int sensor_release(struct inode *node, struct file *f);
ssize_t sensor_read(struct file *flip, char __user *buf, size_t count, loff_t *pos);
ssize_t sensor_write (struct file *flip, const char __user *buf, size_t count, loff_t *pos);
long sensor_ctrl(struct file *flip, unsigned int cmd, unsigned long values);

#endif /*_MPU6050_H*/
