#ifndef _AM335X_H
#define _AM335X_H

#define CM_PER_REG              0x44E00000                  // Direccion base del bloque de registros del clock module Section 8.1.12
#define CM_PER_I2C2_CLKCTRL     0x00000044                  //Section 8.1.12.1.15
#define CM_PER_REG_LEN          CM_PER_I2C2_CLKCTRL + 4     // Longitud del bloque de registros

#define CTRL_MODULE_REG         0x44E10000
#define CTRL_MODULE_REG_LEN     0x00002000
#define CONF_UART1_CSTN         0x00000978
#define CONF_UART1_RSTN         0x0000097C
#define PIN_I2C_CFG             0x00000023


#define I2C2_REG                0x4819C000      // Direccion base fisica del comienzo del bloque de registros de I2C2 Section 21.4.1
#define I2C_REVNB_LO            0x00            // Module Revision Register (low bytes) Section 21.4.1.1
#define I2C_REVNB_HI            0x04            // Module Revision Register (high bytes) Section 21.4.1.2
#define I2C_SYSC                0x10            // System Configuration Register Section 21.4.1.3
#define I2C_IRQSTATUS_RAW       0x24            // I2C Status Raw Register Section 21.4.1.4
#define I2C_IRQSTATUS           0x28            // I2C Status Register Section 21.4.1.5
#define I2C_IRQENABLE_SET       0x2C            // I2C Interrupt Enable Set Register Section 21.4.1.6
#define I2C_IRQENABLE_CLR       0x30            // I2C Interrupt Enable Clear Register Section 21.4.1.7
#define I2C_WE I2C              0x34            // Wakeup Enable Register Section 21.4.1.8
#define I2C_DMARXENABLE_SET     0x38            // Receive DMA Enable Set Register Section 21.4.1.9
#define I2C_DMATXENABLE_SET     0x3C            // Transmit DMA Enable Set Register Section 21.4.1.10
#define I2C_DMARXENABLE_CLR     0x40            // Receive DMA Enable Clear Register Section 21.4.1.11
#define I2C_DMATXENABLE_CLR     0x44            // Transmit DMA Enable Clear Register Section 21.4.1.12
#define I2C_DMARXWAKE_EN        0x48            // Receive DMA Wakeup Register Section 21.4.1.13
#define I2C_DMATXWAKE_EN        0x4C            // Transmit DMA Wakeup Register Section 21.4.1.14
#define I2C_SYSS                0x90            // System Status Register Section 21.4.1.15
#define I2C_BUF                 0x94            // Buffer Configuration Register Section 21.4.1.16
#define I2C_CNT                 0x98            // Data Counter Register Section 21.4.1.17
#define I2C_DATA                0x9C            // Data Access Register Section 21.4.1.18
#define I2C_CON                 0xA4            // I2C Configuration Register Section 21.4.1.19
#define I2C_OA                  0xA8            // I2C Own Address Register Section 21.4.1.20
#define I2C_SA                  0xAC            // I2C Slave Address Register Section 21.4.1.21
#define I2C_PSC                 0xB0            // I2C Clock Prescaler Register Section 21.4.1.22
#define I2C_SCLL                0xB4            // I2C SCL Low Time Register Section 21.4.1.23
#define I2C_SCLH                0xB8            // I2C SCL High Time Register Section 21.4.1.24
#define I2C_SYSTEST             0xBC            // System Test Register Section 21.4.1.25
#define I2C_BUFSTAT             0xC0            // I2C Buffer Status Register Section 21.4.1.26
#define I2C_OA1 I2C             0xC4            // Own Address 1 Register Section 21.4.1.27
#define I2C_OA2 I2C             0xC8            // Own Address 2 Register Section 21.4.1.28
#define I2C_OA3 I2C             0xCC            // Own Address 3 Register Section 21.4.1.29
#define I2C_ACTOA               0xD0            // Active Own Address Register Section 21.4.1.30
#define I2C_SBLOCK              0xD4            // I2C Clock Blocking Enable Register Section 21.4.1.31
#define I2C_REG_LEN             I2C_SBLOCK + 4  // Longitud del bloque de registros

#endif /*_AM335X_H*/
