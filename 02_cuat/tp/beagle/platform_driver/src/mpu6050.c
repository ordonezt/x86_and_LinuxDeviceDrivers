// #include <linux/init.h>             
#include <linux/kernel.h>
// #include <linux/kdev_t.h>
// #include <linux/device.h>
// #include <linux/cdev.h>              
#include <linux/fs.h>               
// #include <linux/module.h>           
// #include <linux/of_address.h>       
// #include <linux/platform_device.h>  
// #include <linux/of.h>               
// #include <linux/io.h>               
// #include <linux/interrupt.h>        
// #include <linux/delay.h>            
// #include <linux/uaccess.h>          
#include <linux/types.h>            
// #include <linux/slab.h>
// #include <linux/version.h>
// #include <linux/of_platform.h>
// #include <linux/wait.h>

int sensor_open(struct inode *node, struct file *f){
    printk(KERN_INFO "Pase por open\n");
    return 0;
}

int sensor_release(struct inode *node, struct file *f){
    printk(KERN_INFO "Pase por close\n");
    return 0;
}

ssize_t sensor_read(struct file *flip, char __user *buf, size_t count, loff_t *pos){
    printk(KERN_INFO "Pase por read\n");
    return 0;
}

ssize_t sensor_write (struct file *flip, const char __user *buf, size_t count, loff_t *pos){
        printk(KERN_INFO "Pase por write\n");
        return 0;
}

long sensor_ctrl(struct file *flip, unsigned int cmd, unsigned long values){
    printk(KERN_INFO "Pase por ioctl\n");
    return 0;
}