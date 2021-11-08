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
#include <linux/jiffies.h>
#include <linux/timer.h>

#include "../inc/TBO_td3_i2c_dev.h"
#include "../inc/am335x.h"
#include "../inc/mpu6050.h"

/* PROTOTIPOS */
static int __init i2c_init(void);
static void __exit i2c_exit(void);
static int cambiar_permisos_cdev(struct device *dev, struct kobj_uevent_env *env);
irqreturn_t I2C_IRQ_Handler(int IRQ, void *ID, struct pt_regs *REG);

static int i2c_probe(struct platform_device * i2c_pd);
static int i2c_remove(struct platform_device * i2c_pd);

int i2c_write(uint8_t address, uint8_t data[], uint16_t count, uint32_t timeout);
uint16_t i2c_read(uint8_t address, uint8_t comando, uint8_t data[], uint16_t count, uint32_t timeout);

int mpu6050_init(void);
int mpu6050_set_register(uint8_t registro, uint8_t valor);
int mpu6050_get_register(uint8_t registro);
int mpu6050_leer_fifo(uint8_t datos[], uint16_t cantidad);

static int sensor_open(struct inode *node, struct file *f);
static int sensor_release(struct inode *node, struct file *f);
static ssize_t sensor_read(struct file *flip, char __user *buf, size_t count, loff_t *pos);
static ssize_t sensor_write (struct file *flip, const char __user *buf, size_t count, loff_t *pos);
static long sensor_ctrl(struct file *flip, unsigned int cmd, unsigned long values);

/* Variables globales*/
static void __iomem *I2C2_Base, *CM_PER_Base, *CTRL_MODULE_Base;    //Mapeo de registros
int virq;                                                           //Interrupcion virtual

struct driver{
    dev_t type;             //Tipo de dispositivo (Numero mayor y menor)
    struct cdev * cdev;     //Manejo del char device
    struct device * device; //TODO
    struct class * class;   //TODO
};

static struct driver my_driver; //Estructura para el manejo del driver

static struct file_operations sensor_fops = {   //File operations del sensor
    .owner          = THIS_MODULE,      //Dueño
    .open           = sensor_open,      //Abrir
    .release        = sensor_release,   //Cerrar
    .read           = sensor_read,      //Leer
    .write          = sensor_write,     //Escribir
    .unlocked_ioctl = sensor_ctrl,      //Control de entrada/salida
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
        //.owner = THIS_MODULE,
        .of_match_table = of_match_ptr(i2c_of_device_ids)
    },
};

static uint8_t *buffer_tx, *buffer_rx;

volatile int wake_up = 0;
wait_queue_head_t wake_up_queue = __WAIT_QUEUE_HEAD_INITIALIZER(wake_up_queue);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Tomás Bautista Ordóñez");
MODULE_VERSION("0.1");
MODULE_DESCRIPTION("Driver I2C");

// MODULE_DEVICE_TABLE(of, i2c_of_device_ids);

module_init(i2c_init);
module_exit(i2c_exit);

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
    uint32_t reg_value_aux;

    pr_info("Driver: Entre al probe\n");

    /**
     * La configuracion consta de los siguientes pasos:
     *  -> Configuracion de los registros necesarios para manejar el I2C en el am335x
     *  -> Configurar el clock de I2C
     *  -> Configurar los pines
     *  -> Configurar el prescaler
     *  -> Sacar el periferico de reset
     *  -> Configurar el modo
     *  -> Habilitar interrupciones
     *  -> Configurar direccion de esclavo
     *  -> Configurar los contadores de datos
     * 
     * Para hacer eso primero es necesario mapear los registros que haya que tocar, habilitar el clock, pedir la/s interupcion/es virtual/es y configurar los pines
     * */

    //Mapeamos el bloque de registros de I2C2
    I2C2_Base = ioremap(I2C2_REG, I2C_REG_LEN);
    if(I2C2_Base == NULL){
        pr_err("Driver: No se pudo mapear el I2C2\n");
        return -1;
    }
    pr_info("Driver: I2C2 mapeado %X\n", (uint32_t)I2C2_Base);
    
    //Mapeamos CM_PER
    CM_PER_Base = ioremap(CM_PER_REG, CM_PER_REG_LEN);
    if(CM_PER_Base == NULL){
        pr_err("Driver: No se pudo mapear CM_PER\n");
        iounmap(I2C2_Base);
        return -1;
    }
    pr_info("Driver: CM_PER mapeado %X\n", (uint32_t)CM_PER_Base);

    //Mapeamos CTRL_MODULE
    CTRL_MODULE_Base = ioremap(CTRL_MODULE_REG, CTRL_MODULE_REG_LEN);  
    if(CTRL_MODULE_Base == NULL){
        pr_err("Driver: No se pudo mapear CTRL_MODULE\n");
        iounmap(I2C2_Base);    
        iounmap(CM_PER_Base);
        return -1;    
    }
    pr_info("Driver: CTRL_MODULE mapeado %X\n", (uint32_t)CTRL_MODULE_Base);

    //Habilitamos el clock del I2C2 seccion 8.1.12.1.15
    reg_value_aux = ioread32(CM_PER_Base + CM_PER_I2C2_CLKCTRL);
    reg_value_aux |= 0x02;
    iowrite32(reg_value_aux,CM_PER_Base + CM_PER_I2C2_CLKCTRL); //MODULEMODE = ENABLE
    msleep(100);
    reg_value_aux = ioread32(CM_PER_Base + CM_PER_I2C2_CLKCTRL);
    if((reg_value_aux & 0x03) != 0x02){
        pr_err("Driver: No se pudo habilitar el clock del I2C2\n");
        iounmap(I2C2_Base);    
        iounmap(CM_PER_Base);    
        iounmap(CTRL_MODULE_Base);    
        return -1;
    }

    //Configuramos los pines como SDA y SCL seccion 9.2.1 y Pagina 1515
    //PIN 20 : UART1_Ctsn - I2C2_SDA
    reg_value_aux = ioread32(CTRL_MODULE_Base + CONF_UART1_CSTN);
    //Reserved | Fast | Receiver Enable | PullDown Selected | PullUp/Pulldown Enable | I2C2_SCL
    //    0       0             1               0                         0            011
    //0x23
    reg_value_aux = PIN_I2C_CFG;
    iowrite32(reg_value_aux, CTRL_MODULE_Base + CONF_UART1_CSTN);

    //PIN 19 : UART1_rtsn - I2C2_SCL
    reg_value_aux = ioread32(CTRL_MODULE_Base + CONF_UART1_RSTN);
    reg_value_aux = PIN_I2C_CFG;
    iowrite32(reg_value_aux,CTRL_MODULE_Base + CONF_UART1_RSTN);
    pr_info("Driver: Clock I2C2 y pines SDA y SCL configurados\n");    

    //Interrupciones virtuales
    virq = platform_get_irq(i2c_pd,0);
    if(virq < 0){
        pr_err("Driver: No pudo obtener la virtual interrupt request\n");
        iounmap(I2C2_Base);    
        iounmap(CM_PER_Base);    
        iounmap(CTRL_MODULE_Base);    
        return -1;
    }
    
    if(request_irq(virq, (irq_handler_t) I2C_IRQ_Handler, IRQF_TRIGGER_RISING, COMPATIBLE, NULL)){
        pr_err("Driver: No se pudo asignar la virtual interrupt request\n");
        iounmap(I2C2_Base);    
        iounmap(CM_PER_Base);    
        iounmap(CTRL_MODULE_Base);    
        return -1;
    }  
    pr_info("Driver: Interrupcion virtual configurada\n");

    pr_info("Driver: Saliendo del probe\n");

    return 0;
}

static int i2c_remove(struct platform_device * i2c_pd){
    pr_info("Driver: Removiendo modulo\n");

    free_irq(virq,NULL);
    pr_info("Driver: Interrupciones virtuales liberadas\n");

    iounmap(I2C2_Base);    
    pr_info("Driver: Registros I2C2 liberados\n");

    iounmap(CM_PER_Base);    
    pr_info("Driver: Registros CM_PER liberados\n");

    iounmap(CTRL_MODULE_Base);
    pr_info("Driver: Registros CTRL_MODULE liberados\n");

    pr_info("Driver: Removi el platform device\n");
    
    return 0;
}

static int sensor_open(struct inode *node, struct file *f){
    uint8_t buff[10]; //TODO borrar
    int aux; //TODO Borrar

    printk(KERN_INFO "Driver: Configurando periferico I2C2 400KHz Master\n");
    /**
     * La configuracion consta de los siguientes pasos:
     *  -> Configuracion de los registros necesarios para manejar el I2C en el am335x
     *  -> Configurar el clock de I2C
     *  -> Configurar los pines
     *  -> Configurar el prescaler
     *  -> Sacar el periferico de reset
     *  -> Configurar el modo
     *  -> Habilitar interrupciones
     *  -> Configurar direccion de esclavo
     *  -> Configurar los contadores de datos
     * */
    printk(KERN_INFO "Driver: Reseteando el modulo\n");

    //Apago el modulo
    iowrite32(0x00, I2C2_Base + I2C_CON);

    //Prescaler para tener clock de 400KHz
    //PSC
    iowrite32(PSC_DIV_400KHz - 1, I2C2_Base + I2C_PSC);
    //SCLL y SCLH
    iowrite32(SCLL_400KHz       , I2C2_Base + I2C_SCLL);
    iowrite32(SCLH_400KHz       , I2C2_Base + I2C_SCLH);

    //Sacar el I2C2 del reset I2C_CON.I2C_EN = 1 y pongo el modulo en modo master I2C_CON.MST = 1
    iowrite32(0x8400, I2C2_Base + I2C_CON);

    printk(KERN_INFO "Driver: Configurando sensor\n");
    if(mpu6050_init()){
        pr_err("Driver: Error configurando el sensor\n");
        return -1;
    }
    printk(KERN_INFO "Driver: Sensor configurado exitosamente\n");

    return 0;
}

static int sensor_release(struct inode *node, struct file *f){
    printk(KERN_INFO "Pase por close\n");
    return 0;
}

static ssize_t sensor_read(struct file *flip, char __user *buf, size_t count, loff_t *pos){
    // uint8_t *datos;
    // int cant_leida, cant_no_copiada;

    printk(KERN_INFO "Pase por read\n");
    // datos = (uint8_t *)kmalloc(count,GFP_KERNEL);
    // cant_leida = mpu6050_leer_fifo(datos, count);
    // cant_no_copiada = copy_to_user(buf, datos, cant_leida);
    // kfree(datos);
    return 0;
    // return cant_leida - cant_no_copiada;
}

static ssize_t sensor_write (struct file *flip, const char __user *buf, size_t count, loff_t *pos){
        printk(KERN_INFO "Pase por write\n");
        return 0;
}

static long sensor_ctrl(struct file *flip, unsigned int cmd, unsigned long values){
    printk(KERN_INFO "Pase por ioctl\n");
    return 0;
}

/**
 * @brief Escribe en el bus I2C
 * 
 * @param address Direccion del dispositivo a escribir
 * @param data Direccion de los datos a transmitir
 * @param count Cantidad de datos a transmitir
 * @param timeout Timeout en ms de espera a que se libere el bus
 * @return int Cantidad de datos transmitidos
 */
int i2c_write(uint8_t address, uint8_t data[], uint16_t count, uint32_t timeout){
    uint32_t timeout_aux = 0;
    /*
        I2C Write
        ┌──────┬─┬─────┬───┬─────┬───┬────┬───┬────┬───┬───┬────┬───┬─┐
        │Master│S│Adr+W│   │R-Adr│   │Data│   │Data│   │...│Data│   │P│
        ├──────┼─┼─────┼───┼─────┼───┼────┼───┼────┼───┼───┼────┼───┼─┤
        │Slave │ │     │ACK│     │ACK│    │ACK│    │ACK│...│    │ACK│ │
        └──────┴─┴─────┴───┴─────┴───┴────┴───┴────┴───┴───┴────┴───┴─┘
    */
    //Si el bus esta ocupado espero que se libere
    while(ioread32(I2C2_Base + I2C_IRQSTATUS_RAW) & 0x1000){
        msleep(1);
        timeout_aux++;
        if((timeout_aux > timeout) && (timeout != 0)){
            pr_info("Driver: Bus ocupado\n");
            return -1;
        }
    }

    //Deshabilito todas las interrupciones
    iowrite32(0xFFFF, I2C2_Base + I2C_IRQENABLE_CLR);
    //Habilito la interrupcion por acceso listo (transaccion completa) y dato transmitido
    iowrite32(ARDY_MASK, I2C2_Base + I2C_IRQENABLE_SET);
    iowrite32(XRDY_MASK, I2C2_Base + I2C_IRQENABLE_SET);

    //Le indico al modulo cuantos datos vamos a transmitir
    iowrite32(count, I2C2_Base + I2C_CNT);

    //Escribimos la direccion del esclavo objetivo
    iowrite32(address, I2C2_Base + I2C_SA);

    //Apuntamos al vector que contiene los datos a transmitir
    buffer_tx = data;
    //Escribimos el primer dato
    iowrite32(*buffer_tx, I2C2_Base + I2C_DATA);

    //Establecemos el tipo de transaccion en transmision master con bit de start y con bit de stop
    iowrite32(0x8603, I2C2_Base + I2C_CON);

    //Espero que se complete la transmision
    //pr_info("Driver: Mandando %d bytes a %X. Esperando...\n", 1, address);
    if(wait_event_interruptible(wake_up_queue, wake_up > 0) < 0){
        wake_up = 0;
        pr_err("Driver: Error en la espera de transmision\n");
        return count - ioread32(I2C2_Base + I2C_CNT);
    }
    wake_up = 0;

    return count;
}

/**
 * @brief Lee el bus i2c
 * 
 * @param address Direccion del dispositivo a leer
 * @param comando Comando que se quiere leer
 * @param data Direccion para alojar la lectura
 * @param count Cantidad de datos a leer
 * @param timeout Tiempo maximo de espera en ms que se espera que se libere el bus.
 * @return uint16_t Cantidad de datos leidos
 */
uint16_t i2c_read(uint8_t address, uint8_t comando, uint8_t data[], uint16_t count, uint32_t timeout){
    uint32_t timeout_aux = 0;
    /*
        I2C Read
        ┌──────┬─┬─────┬───┬─────┬───┬─┬─────┬───┬────┬───┬───┬────┬────┬─┐
        │Master│S│Adr+W│   │R-Adr│   │S│Adr+R│   │    │ACK│...│    │NACK│P│
        ├──────┼─┼─────┼───┼─────┼───┼─┼─────┼───┼────┼───┼───┼────┼────┼─┤
        │Slave │ │     │ACK│     │ACK│ │     │ACK│Data│   │...│Data│    │ │
        └──────┴─┴─────┴───┴─────┴───┴─┴─────┴───┴────┴───┴───┴────┴────┴─┘
    */
    
    //Si el bus esta ocupado espero que se libere
    while(ioread32(I2C2_Base + I2C_IRQSTATUS_RAW) & 0x1000){
        msleep(1);
        timeout_aux++;
        if((timeout_aux > timeout) && (timeout != 0)){
            pr_info("Driver: Bus ocupado\n");
            return -1;
        }
    }
    //Bus libre, comienzo la escritura

    //Deshabilito todas las interrupciones
    iowrite32(0xFFFF, I2C2_Base + I2C_IRQENABLE_CLR);
    //Habilito la interrupcion por acceso listo (transaccion completa)
    iowrite32(ARDY_MASK, I2C2_Base + I2C_IRQENABLE_SET);

    //Le indico al modulo cuantos datos vamos a transmitir (1 solo, la direccion a leer)
    iowrite32(1, I2C2_Base + I2C_CNT);

    //Escribimos la direccion del esclavo objetivo
    iowrite32(address, I2C2_Base + I2C_SA);

    //Escribimos la direccion que queremos leer
    iowrite32(comando, I2C2_Base + I2C_DATA);

    //Establecemos el tipo de transaccion en transmision master con bit de start sin bit de stop
    iowrite32(0x8601, I2C2_Base + I2C_CON);

    //Espero que se complete la transmision
    //pr_info("Driver: Mandando %d bytes a %X. Esperando...\n", 1, address);
    if(wait_event_interruptible(wake_up_queue, wake_up > 0) < 0){
        wake_up = 0;
        pr_err("Driver: Error en la espera de transmision\n");
        return -1;
    }
    wake_up = 0;

    //Habilitamos la interrupcion por recepcion (para poder ir guardando los datos leidos)
    iowrite32(RRDY_MASK, I2C2_Base + I2C_IRQENABLE_SET);

    //Le indico al modulo cuantos datos vamos a recibir
    iowrite32(count, I2C2_Base + I2C_CNT);

    //Apunto el buffer al destino de los datos
    buffer_rx = data;

    //Establecemos el tipo de transaccion en recepcion master con bit de start y bit de stop (cuando se complete la transaccion)
    iowrite32(0x8403, I2C2_Base + I2C_CON);

    //Espero que se complete la recepcion
    //pr_info("Driver: Recibiendo %d bytes a %X. Esperando...\n", 1, address);
    if(wait_event_interruptible(wake_up_queue, wake_up > 0) < 0){
        wake_up = 0;
        pr_err("Driver: Error en la espera de transmision\n");
        return count - ioread32(I2C2_Base + I2C_CNT);
    }
    wake_up = 0;

    //pr_info("Driver: Recepcion exitosa\n");
    return count;
}

/**
 * @brief Maneja la interrupcion de I2C
 * 
 * @param IRQ 
 * @param ID 
 * @param REG 
 * @return irqreturn_t 
 */
irqreturn_t I2C_IRQ_Handler(int IRQ, void *ID, struct pt_regs *REG){
    int irq_status;
    //pr_info("Driver: Llego una interrupcion\n");

    //Averiguo por que motivos llego la interrupcion
    irq_status = ioread32(I2C2_Base + I2C_IRQSTATUS);

    if(irq_status & XRDY_MASK){     //Interrupcion por transferencia completada
        //pr_info("Driver: Era por escritura\n");

        //Si hay mas datos seguimos transmitiendo
        if(ioread32(I2C2_Base + I2C_CNT) > 1){
            buffer_tx++;
            iowrite32(*buffer_tx, I2C2_Base + I2C_DATA);
        }

        //Limpio la interrupcion
        iowrite32(XRDY_MASK, I2C2_Base + I2C_IRQSTATUS);
    }
    
    if(irq_status & RRDY_MASK){
        //pr_info("Driver: Era por lectura\n");

        //Leo el dato y aumento la posicion
        *buffer_rx++ = ioread32(I2C2_Base + I2C_DATA);

        //Limpio la interrupcion
        iowrite32(RRDY_MASK, I2C2_Base + I2C_IRQSTATUS);

        //Leo la cantidad de datos restantes
        //pr_info("Driver: Llego un byte. Bytes pendientes: %d\n", ioread32(I2C2_Base + I2C_CNT));
    }

    if(irq_status & ARDY_MASK){
        //pr_info("Driver: Era por fin de transaccion\n");

        //Limpio la interrupcion
        iowrite32(ARDY_MASK, I2C2_Base + I2C_IRQSTATUS);

        //Despierto el programa
        wake_up = 1;
        wake_up_interruptible(&wake_up_queue);

        return IRQ_HANDLED;
    }

    return IRQ_HANDLED;
}

int mpu6050_init(void){
    //Reset
    if(mpu6050_set_register(MPU6050_RA_PWR_MGMT_1, 1<<7))
        return -1;
    
    //Salimos del sleep
    if(mpu6050_set_register(MPU6050_RA_PWR_MGMT_1, 1))
        return -1;
    
    //Frecuencia de muestreo 200Hz
    // if(mpu6050_set_register(MPU6050_RA_SMPLRT_DIV, 0xFF))
    //     return -1;
    if(mpu6050_set_register(MPU6050_RA_CONFIG, 3))
        return -1;
    if(mpu6050_set_register(MPU6050_RA_SMPLRT_DIV, 4))
        return -1;
    
    //Configuracion de la fifo
    if(mpu6050_set_register(MPU6050_RA_USER_CTRL, 0))
        return -1;
    if(mpu6050_set_register(MPU6050_RA_USER_CTRL, 0x40))
        return -1;
    if(mpu6050_set_register(MPU6050_RA_FIFO_EN, 0xF8))
        return -1;

    return 0;
}

int mpu6050_set_register(uint8_t registro, uint8_t valor){
    uint8_t buff[2];

    buff[0] = registro;
    buff[1] = valor;
    if(i2c_write(MPU6050_ADDRESS, buff, 2, 10) != 2)
        return -1;
    return 0;
}

int mpu6050_get_register(uint8_t registro){
    uint8_t valor;

    if(i2c_read(MPU6050_ADDRESS, registro, &valor, 1, 10) != 1)
        return -1;
    else
        return valor;
}

int mpu6050_get_fifo_count(void){
    int aux;
    
    aux = mpu6050_get_register(MPU6050_RA_FIFO_COUNTH) << 8;
    aux |= mpu6050_get_register(MPU6050_RA_FIFO_COUNTL);

    return aux;
}

int mpu6050_leer_fifo(uint8_t datos[], uint16_t cantidad){
    int datos_restantes, cant_actual;
    //TODO falta contemplar si me piden mas de 1024 bytes
    //Me fijo si la FIFO se desbordo. En caso de que si reinicio el muestreo
    if(mpu6050_get_register(MPU6050_RA_INT_STATUS) & 0x10){
        pr_info("Driver: FIFO desbordada\n");
        //Reiniciar muestreo
        mpu6050_set_register(MPU6050_RA_USER_CTRL, 0x04);
        mpu6050_set_register(MPU6050_RA_USER_CTRL, 0x40);

        datos_restantes = cantidad;
    }

    cant_actual = mpu6050_get_fifo_count();
    pr_info("Driver: La FIFO tiene %d bytes", cant_actual);
    datos_restantes = cantidad - cant_actual;

    if(datos_restantes > 0){
        pr_info("Driver: Faltan %d datos", datos_restantes);
        msleep(T_MUESTREO_MS * datos_restantes / 7);
    }
    
    //Ahora ya estoy seguro de que la fifo tiene la cantidad necesaria de datos
    return i2c_read(MPU6050_ADDRESS, MPU6050_RA_FIFO_R_W, datos, cantidad, 10);
}
