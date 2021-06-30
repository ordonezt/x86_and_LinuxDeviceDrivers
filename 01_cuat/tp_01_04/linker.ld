ENTRY (reset)

__EH_FRAME              = 0x00060000; /*seccion para c y c++ exception handling*/
__DATA_NO_INIT          = 0x00050000; 

/*-----------------VMA-----------------*/
/*Rutinas*/ 
__RUTINAS_VMA           = 0x0010000; /*tengo los saltos referenciados a 0*/

/*Rutinas ROM*/ 
__RUTINAS_ROM_VMA       = 0xFFFF3000;

/*Kernel*/
__KERNEL_VMA            = 0x00202000;

/*Pila*/
__STACK_START           = 0x2FFF8000; 
__STACK_END             = 0x2FFF8FFF;
__STACK_SIZE            = ((__STACK_END - __STACK_START) / 4);

/*Init ROM*/
__ROM_VMA               = 0xFFFF0000;

__TABLAS_SISTEMA_VMA    = 0xFFFFFD00; /*donde guardo la GDT*/

/*Reset*/
__RESET_VMA             = 0xFFFFFFF0;

/*-----------------LMA-----------------*/
__ROM_LMA               = 0xFFFF0000; 

__RUTINAS_LMA           = 0xFFFF3000;
__RUTINAS_ROM_LMA       = 0xFFFF3000;

__KERNEL_LMA            = 0xFFFF2000;

__TABLAS_SISTEMA_LMA    = 0xFFFFFD00;

__RESET_LMA             = 0xFFFFFFF0;

MEMORY
{
    ram (!x) : ORIGIN = 0x00000000, LENGTH = 0xFFFF0000 /*2 a la 32 (4GB) - 2 a la 16 (64KB)*/
    rom (rx) : ORIGIN = 0xFFFF0000, LENGTH = 64K
}

SECTIONS
{
    /* .rutinas_ram __RUTINAS_VMA : AT (__RUTINAS_LMA) {
            *(.rutinas);
    } > ram */

    .rutinas_rom __RUTINAS_ROM_VMA : AT (__RUTINAS_LMA) {
            *(.rutinas);
            *(.note.gnu.property);
    } > rom
    __rutinas_size = SIZEOF(.rutinas_rom);

    .kernel __KERNEL_VMA : AT (__KERNEL_LMA) {
            *(.kernel);
    } > ram
    __kernel_size = SIZEOF(.kernel);


    /* Mapeo de la ROM */
    .init_rom __ROM_VMA : AT (__ROM_LMA) {
            *(.rom_init);
    } > rom

    /* Mapeo de las tablas del sistema */
    .tablas_sistema __TABLAS_SISTEMA_VMA : AT (__TABLAS_SISTEMA_LMA) {
            *(.tablas_sistema);
    } > rom

    /* Mapeo de la RAM */
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