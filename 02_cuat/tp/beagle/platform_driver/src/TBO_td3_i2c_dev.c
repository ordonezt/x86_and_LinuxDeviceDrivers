#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/kdev_t.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/of_address.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/io.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/version.h>
#include <linux/of_platform.h>
#include <linux/wait.h>

#include "../inc/TBO_td3_i2c_dev.h"
#include "../inc/mpu6050.h"

/* PROTOTIPOS */
static int __init i2c_init(void);
static void __exit i2c_exit(void);
static int cambiar_permisos_cdev(struct device *dev, struct kobj_uevent_env *env);

static int i2c_probe(struct platform_device * i2c_pd);
static int i2c_remove(struct platform_device * i2c_pd);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Tomás Bautista Ordóñez");
MODULE_VERSION("0.1");
MODULE_DESCRIPTION("Driver I2C");

// MODULE_DEVICE_TABLE(of, i2c_of_device_ids);

module_init(i2c_init);
module_exit(i2c_exit);

struct driver{
    dev_t type;             //Tipo de dispositivo (Numero mayor y menor)
    struct cdev * cdev;     //Manejo del char device
    struct device * device; //TODO
    struct class * class;   //TODO
};

static struct driver my_driver; //Estructura para el manejo del driver

static struct file_operations sensor_fops = {   //File operations del sensor
    .owner = THIS_MODULE,           //Dueño
    .open = sensor_open,            //Abrir
    .release = sensor_release,      //Cerrar
    .read = sensor_read,            //Leer
    .write = sensor_write,          //Escribir
    .unlocked_ioctl = sensor_ctrl,  //Control de entrada/salida
};

static struct of_device_id i2c_of_device_ids[] = {
    {
        .compatible = COMPATIBLE,
    }, {},
};

static struct platform_driver i2c_platform_driver = { //Handlers del platform driver
    .probe = i2c_probe,     //Instalacion
    .remove = i2c_remove,   //Remocion
    .driver = {             //Driver objetivo
        .name = COMPATIBLE,
        .owner = THIS_MODULE,
        .of_match_table = of_match_ptr(i2c_of_device_ids)
    },
};

/**
 * @brief Instala el modulo driver I2C en el sistema
 * 
 * @return int 0 exito, negativo error
 */
static int __init i2c_init(void){
    int retorno = 0;

    printk(KERN_INFO "Driver: Instalando el driver\n");

    //Pedimos al sistema una estructura cdev
    if((my_driver.cdev = cdev_alloc()) == NULL){
        printk(KERN_ERR "Driver: No se pudo allocar el cdev\n");
        return -1;
    }
    printk(KERN_INFO "Driver: cdev allocado\n");

    //Pedimos un numero mayor al sistema
    if((retorno = alloc_chrdev_region(&my_driver.type, PRIMER_MENOR, CANT_DISP, CDEV_NAME)) < 0){
        printk(KERN_ERR "Driver: No se pudo asignar un numero mayor\n");
        cdev_del(my_driver.cdev);
        return retorno;
    }
    printk(KERN_INFO "Driver: Numero mayor asignado %d\n", MAJOR(my_driver.type));

    //Asigno las file operations
    cdev_init(my_driver.cdev, &sensor_fops);
    printk(KERN_INFO "Driver: File operations asignadas\n");

    //Agrego el char device al sistema
    if((retorno = cdev_add(my_driver.cdev, my_driver.type, CANT_DISP)) < 0){
        printk(KERN_ERR "Driver: No se pudo agregar el char device\n");
        unregister_chrdev_region(my_driver.type, CANT_DISP);
        cdev_del(my_driver.cdev);
        return retorno;
    }
    printk(KERN_INFO "Driver: Char device agregado\n");

    //Pido al sistema una estructura class
    if((my_driver.class = class_create(THIS_MODULE, CLASS_NAME)) == NULL){
        printk(KERN_ERR "Driver: No se pudo crear el class\n");
        unregister_chrdev_region(my_driver.type, CANT_DISP);
        cdev_del(my_driver.cdev);
        return -1;
    }
    printk(KERN_INFO "Driver: Se creo un class\n");

    //Cambio los permisos del .ko
    my_driver.class->dev_uevent = cambiar_permisos_cdev;

    //Creo el device
    if((device_create(my_driver.class, NULL, my_driver.type, NULL, COMPATIBLE) == NULL)){
        printk(KERN_ERR "Driver: No se pudo crear el device\n");
        class_destroy(my_driver.class);
        unregister_chrdev_region(my_driver.type, CANT_DISP);
        cdev_del(my_driver.cdev);
        return -1;
    }
    printk(KERN_INFO "Driver: Se creo un device\n");

    //Registro el platform driver
    if((retorno = platform_driver_register(&i2c_platform_driver)) < 0){
        printk(KERN_ERR "Driver: No se pudo registrar el plaftform driver\n");
        device_destroy(my_driver.class, my_driver.type);
        class_destroy(my_driver.class);
        unregister_chrdev_region(my_driver.type, CANT_DISP);
        cdev_del(my_driver.cdev);
        return -1;
    }
    printk(KERN_INFO "Driver: Se registro el platform driver\n");

    return 0;
}

/**
 * @brief Desinstala el modulo I2C del sistema
 * 
 */
static void __exit i2c_exit(void){
    //TODO El orden importa? los chicos lo hicieron distinto
    printk(KERN_INFO "Driver: Cerrando el char device\n");

    //Deslinkeo el platform driver
    platform_driver_unregister(&i2c_platform_driver);
    
    //Destruyo el device
    device_destroy(my_driver.class, my_driver.type);

    //Destruyo el class
    class_destroy(my_driver.class);
    
    //Deslinkeo el char device
    unregister_chrdev_region(my_driver.type, CANT_DISP);
    
    //Borro el char device
    cdev_del(my_driver.cdev);

    printk(KERN_INFO "Driver: Char device cerrado\n");
}

/**
 * @brief Evento de device_create
 * Cambia los permisos del .ko para poder instalar el modulo
 *
 * @param dev 
 * @param env 
 * @return int 0
 */
static int cambiar_permisos_cdev(struct device *dev, struct kobj_uevent_env *env){
    add_uevent_var(env, "DEVMODE=%#o", 0666);
    return 0;
}

static int i2c_probe(struct platform_device * i2c_pd){
    pr_info("Driver: Entre al probe\n");

    //Mapeo de registros

    return 0;
}

static int i2c_remove(struct platform_device * i2c_pd){

    pr_info("Driver: Removi el platform device\n");
    
    return 0;
}

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
