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
/*Interrupciones*/ 
__INTERRUPCIONES_VMA    = 0x00100000;

/*Rutinas*/ 
__RUTINAS_VMA           = 0x0010000; /*tengo los saltos referenciados a 0*/

/*Rutinas ROM*/ 
__RUTINAS_ROM_VMA       = 0xFFFF3000;

/*Kernel*/
__KERNEL_VMA            = 0x00202000;

/*Pila 32*/
__STACK_START           = 0x2FFF8000; 
__STACK_END             = 0x2FFF8FFF;
__STACK_SIZE            = ((__STACK_END - __STACK_START) / 4);

/*Init ROM*/
__ROM_VMA               = 0xFFFF0000;

__TABLAS_SISTEMA_16_VMA = 0xFFFFFD00;

__TABLAS_SISTEMA_VMA    = 0x00000000; /*donde guardo la GDT*/

/*Reset*/
__RESET_VMA             = 0xFFFFFFF0;

/*-----------------LMA-----------------*/
__ROM_LMA               = 0xFFFF0000; 

__KERNEL_LMA            = 0xFFFF2000;

__RUTINAS_ROM_LMA       = 0xFFFF3000;

/*Interrupciones*/ 
__INTERRUPCIONES_LMA    = 0xFFFF3100;

__RUTINAS_LMA           = 0xFFFF4000;

__TABLAS_SISTEMA_16_LMA = 0xFFFFFD00;

__TABLAS_SISTEMA_LMA    = 0xFFFFFC00;

__RESET_LMA             = 0xFFFFFFF0;


/*Pila 16*/
__STACK_16_SIZE         = 0xFF;
__STACK_16_END          = __RESET_LMA;
__STACK_16_START        = __STACK_16_END - __STACK_16_SIZE; 

MEMORY
{
    ram (!x) : ORIGIN = 0x00000000, LENGTH = 0xFFFF0000 /*2 a la 32 (4GB) - 2 a la 16 (64KB)*/
    rom (rx) : ORIGIN = 0xFFFF0000, LENGTH = 64K
}

SECTIONS
{
        /* Mapeo de las rutinas en RAM */
        .rutinas_ram __RUTINAS_VMA : AT (__RUTINAS_LMA) {
                *(.rutinas);
        } > ram
        __rutinas_size = SIZEOF(.rutinas_ram);

        /* Mapeo de las interrupciones y teclado en RAM */
        .interrupciones __INTERRUPCIONES_VMA : AT (__INTERRUPCIONES_LMA) {
                *(.interrupciones);
        } > ram
        __interrupciones_size = SIZEOF(.interrupciones);

        /* Mapeo del kernel en RAM*/
        .kernel __KERNEL_VMA : AT (__KERNEL_LMA) {
                *(.kernel);
        } > ram
        __kernel_size = SIZEOF(.kernel);

        /* Mapeo de las funciones auxiliares en ROM */
        .rutinas_rom __RUTINAS_ROM_VMA : AT (__RUTINAS_ROM_LMA) {
                *(.rutinas_rom);
                *(.note.gnu.property);
        } > rom

        /* Mapeo de la ROM */
        .init_rom __ROM_VMA : AT (__ROM_LMA) {
                *(.rom_init);
        } > rom

        /* Mapeo de las tablas del sistema */
        .tablas_sistema_16 __TABLAS_SISTEMA_16_VMA : AT (__TABLAS_SISTEMA_16_LMA) {
                *(.tablas_sistema_16);
        } > rom

        .tablas_sistema __TABLAS_SISTEMA_VMA : AT (__TABLAS_SISTEMA_LMA) {
                *(.tablas_sistema);
        } > ram
        __tablas_sistema_size = SIZEOF(.tablas_sistema);


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