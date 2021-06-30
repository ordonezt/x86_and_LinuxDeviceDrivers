ENTRY (reset)

__EH_FRAME              = 0x00060000; /*seccion para c y c++ exception handling*/
__DATA_NO_INIT          = 0x00050000; 

/*--------------Selectores-------------*/
__CS_BASE               = 0x00000000;
__CS_LIMITE             = 0xFFFFFFFF;

__DS_BASE               = 0x00000000;
__DS_LIMITE             = 0xFFFFFFFF;

__SS_BASE               = 0x00000000;
__SS_LIMITE             = 0xFFFFFFFF;


/*-----------------VMA-----------------*/
/*Tablas de sistema*/ 
__TABLAS_SISTEMA_VMA    = 0x00000000; /*donde guardo la GDT e IDT*/

/*Rutinas*/ 
__RUTINAS_VMA           = 0x00010000; /*donde se van a ejecutar las funciones de uso general*/

/*Interrupciones*/ 
__INTERRUPCIONES_VMA    = 0x00100000; /*donde se van a ejecutar los handlers de int*/

/*Tabla de digitos*/
__TABLA_DIGITOS_VMA     = 0x00200000; /*donde se almacenan los digitos*/

/*Datos*/ 
__DATOS_VMA             = 0x00210000; /*donde guardo datos y variables globales*/

/*Kernel*/
__KERNEL_VMA            = 0x00202000; /*donde se va a ejecutar el kernel*/

/*Pila 32*/
__STACK_START           = 0x2FFF8000; /*donde se va crear la pila*/
__STACK_END             = 0x0;
__STACK_SIZE            = ((__STACK_START - __STACK_END) / 4);


/*Init ROM*/
__ROM_VMA               = 0xFFFF0000;

/*Rutinas ROM*/ 
__RUTINAS_ROM_VMA       = 0xFFFF1500; /*donde guardo las rutinas que se ejecutan en rom*/

/*Tablas de sistema modo real*/
__TABLAS_SISTEMA_16_VMA = 0xFFFFFD00; /*donde guardo la gdt de modo real*/

/*Reset*/
__RESET_VMA             = 0xFFFFFFF0; /*direccion de arranque*/

/*-----------------LMA-----------------*/
__ROM_LMA               = 0xFFFF0000; 

__KERNEL_LMA            = 0xFFFF1000;

__RUTINAS_ROM_LMA       = 0xFFFF1500;

/*Interrupciones*/ 
__INTERRUPCIONES_LMA    = 0xFFFF3000;

/*Tabla de digitos*/
__TABLA_DIGITOS_LMA     = 0xFFFF3700;

/*Datos*/ 
__DATOS_LMA             = 0xFFFF3800;

__RUTINAS_LMA           = 0xFFFF4000;

__TABLAS_SISTEMA_LMA    = 0xFFFFFC00;

__TABLAS_SISTEMA_16_LMA = 0xFFFFFD00;

__RESET_LMA             = 0xFFFFFFF0;


MEMORY
{
    ram (!x) : ORIGIN = 0x00000000, LENGTH = 0xFFFF0000 /*2 a la 32 (4GB) - 2 a la 16 (64KB)*/
    rom (rx) : ORIGIN = 0xFFFF0000, LENGTH = 64K
}

SECTIONS
{
        /* Mapeo de las tablas de sistema en RAM */
        .tablas_sistema __TABLAS_SISTEMA_VMA : AT (__TABLAS_SISTEMA_LMA) {
                *(.tablas_sistema);
        } > ram
        __tablas_sistema_size = SIZEOF(.tablas_sistema);

        /* Mapeo de las interrupciones y teclado en RAM */
        .interrupciones __INTERRUPCIONES_VMA : AT (__INTERRUPCIONES_LMA) {
                *(.interrupciones);
        } > ram
        __interrupciones_size = SIZEOF(.interrupciones);

        /* Mapeo de las rutinas en RAM */
        .rutinas_ram __RUTINAS_VMA : AT (__RUTINAS_LMA) {
                *(.rutinas);
        } > ram
        __rutinas_size = SIZEOF(.rutinas_ram);

        /* Mapeo de los datos en RAM */
        .tabla_digitos __TABLA_DIGITOS_VMA : AT (__TABLA_DIGITOS_LMA) {
                *(.tabla_digitos);
        } > ram

        /* Mapeo de los datos en RAM */
        .datos __DATOS_VMA : AT (__DATOS_LMA) {
                *(.datos);
                *(.data);
                *(.bss);
        } > ram

        /* Mapeo del kernel en RAM*/
        .kernel __KERNEL_VMA : AT (__KERNEL_LMA) {
                *(.kernel);
        } > ram
        __kernel_size = SIZEOF(.kernel);

        /* Mapeo de la ROM */
        .init_rom __ROM_VMA : AT (__ROM_LMA) {
                *(.rom_init);
                *(.text);
                *(.rodata);
        } > rom

        /* Mapeo de las funciones auxiliares en ROM */
        .rutinas_rom __RUTINAS_ROM_VMA : AT (__RUTINAS_ROM_LMA) {
                *(.rutinas_rom);
                *(.note.gnu.property);
        } > rom

        /* Mapeo de las tablas del sistema de modo real*/
        .tablas_sistema_16 __TABLAS_SISTEMA_16_VMA : AT (__TABLAS_SISTEMA_16_LMA) {
                *(.tablas_sistema_16);
        } > rom

        /* Mapeo del reset */
        .reset __RESET_VMA : AT (__RESET_LMA) {
                *(.reset_vector);
        } > rom

        /* Relleno de la ROM */
        .fill_rom : {
                FILL(0x90);
                . = ORIGIN(rom) + LENGTH(rom)-1;
                BYTE(0x90);
        } > rom
}