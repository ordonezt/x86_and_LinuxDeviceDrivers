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

__LONGITUD_RAM          = 512M;

/*-----------------VMA-----------------*/
/*Tablas de sistema*/ 
__TABLAS_SISTEMA_VMA_FISICA    = 0x00000000; /*donde guardo la GDT e IDT*/
__TABLAS_SISTEMA_VMA_LINEAL    = 0x00000000;

/*Tablas de paginacion*/ 
__TABLAS_PAGINACION_VMA_FISICA = 0x00010000; /*donde guardo las DTPs y PTs*/
__TABLAS_PAGINACION_VMA_LINEAL = 0x00010000; /*donde guardo las DTPs y PTs*/

/*Rutinas*/ 
__RUTINAS_VMA_FISICA           = 0x00050000; /*donde se van a ejecutar las funciones de uso general*/
__RUTINAS_VMA_LINEAL           = 0x00050000; /*donde se van a ejecutar las funciones de uso general*/

/*Sección de RAM de video*/
__VIDEO_VMA_FISICA             = 0x000B8000;
__VIDEO_VMA_LINEAL             = 0x00E80000;

/*Interrupciones*/ 
__INTERRUPCIONES_VMA_FISICA    = 0x00100000; /*donde se van a ejecutar los handlers de int*/
__INTERRUPCIONES_VMA_LINEAL    = 0x00100000; /*donde se van a ejecutar los handlers de int*/

/*Tabla de digitos*/
__TABLA_DIGITOS_VMA_FISICA     = 0x00210000; /*donde se almacenan los digitos*/
__TABLA_DIGITOS_VMA_LINEAL     = 0x01210000; /*donde se almacenan los digitos*/

/*Datos*/ 
__DATOS_VMA_FISICA             = 0x00200000; /*donde guardo datos y variables globales*/
__DATOS_VMA_LINEAL             = 0x01200000; /*donde guardo datos y variables globales*/

/*Kernel*/
__KERNEL_VMA_FISICA            = 0x00220000; /*donde se va a ejecutar el kernel*/
__KERNEL_VMA_LINEAL            = 0x01220000; /*donde se va a ejecutar el kernel*/

/*Tarea 1*/
__TAREA_1_VMA_FISICA           = 0x00310000;
__TAREA_1_VMA_LINEAL           = 0x01310000;
__TAREA_1_TEXT_VMA_FISICA      = __TAREA_1_VMA_FISICA;
__TAREA_1_TEXT_VMA_LINEAL      = __TAREA_1_VMA_LINEAL;
__TAREA_1_BSS_VMA_FISICA       = 0x00320000;
__TAREA_1_BSS_VMA_LINEAL       = 0x01320000;
__TAREA_1_DATA_VMA_FISICA      = 0x00330000;
__TAREA_1_DATA_VMA_LINEAL      = 0x01330000;
__TAREA_1_RODATA_VMA_FISICA    = 0x00340000;
__TAREA_1_RODATA_VMA_LINEAL    = 0x01340000;

/*Pila 32*/
__KERNEL_PILA_FINAL_FISICA     = 0x1FFF8000; /*donde se va crear la pila*/
__KERNEL_PILA_FINAL_LINEAL     = 0x1FFF8000; /*donde se va crear la pila*/
__KERNEL_PILA_INICIO_FISICA    = __KERNEL_PILA_FINAL_FISICA + 4K; /*donde se va crear la pila*/
__KERNEL_PILA_INICIO_LINEAL    = __KERNEL_PILA_FINAL_LINEAL + 4K; /*donde se va crear la pila*/
__TAREA_1_PILA_FINAL_FISICA    = 0x1FFFF000;
__TAREA_1_PILA_FINAL_LINEAL    = 0x0078F000;
__TAREA_1_PILA_INICIO_FISICA   = __TAREA_1_PILA_FINAL_FISICA + 4K;
__TAREA_1_PILA_INICIO_LINEAL   = __TAREA_1_PILA_FINAL_LINEAL + 4K;

/*Direccion fisica en lña que arranco a mapear paginas no presentes*/
__PAGINACION_DINAMICA_BASE_FISICA = 0x0A000000;

/*Init ROM*/
__ROM_VMA_FISICA               = 0xFFFF0000;
__ROM_VMA_LINEAL               = 0xFFFF0000;

/*Rutinas ROM*/ 
__RUTINAS_ROM_VMA_FISICA       = 0xFFFF1500; /*donde guardo las rutinas que se ejecutan en rom*/
__RUTINAS_ROM_VMA_LINEAL       = 0xFFFF1500; /*donde guardo las rutinas que se ejecutan en rom*/

/*Tablas de sistema modo real*/
__TABLAS_SISTEMA_16_VMA_FISICA = 0xFFFFFD00; /*donde guardo la gdt de modo real*/
__TABLAS_SISTEMA_16_VMA_LINEAL = 0xFFFFFD00; /*donde guardo la gdt de modo real*/

/*Reset*/
__RESET_VMA_FISICA             = 0xFFFFFFF0; /*direccion de arranque*/
__RESET_VMA_LINEAL             = 0xFFFFFFF0; /*direccion de arranque*/

/*-----------------LMA-----------------*/
/* __ROM_LMA               = 0xFFFF0000; */

/* __KERNEL_LMA            = 0xFFFF1000; */

/*__RUTINAS_ROM_LMA       = 0xFFFF1500; */

/*Interrupciones*/ 
/*__INTERRUPCIONES_LMA    = 0xFFFF3000; */

/*Tabla de digitos*/
/*__TABLA_DIGITOS_LMA     = 0xFFFF3700; */

/*Datos*/ 
/*__DATOS_LMA             = 0xFFFF3800; */

/*__RUTINAS_LMA           = 0xFFFF4000; */

/*Tarea 1*/
/*__TAREA_1_LMA           = 0xFFFF5F00; */

/*Video*/
/*__VIDEO_LMA             = 0xFFFF6000; */

/*__TABLAS_SISTEMA_LMA    = 0xFFFFFC00; */
__TABLAS_SISTEMA_LMA    = __ROM_VMA_FISICA;

/*__TABLAS_SISTEMA_16_LMA = 0xFFFFFD00; */

__RESET_LMA             = 0xFFFFFFF0;


MEMORY
{
    ram (!x) : ORIGIN = 0x00000000, LENGTH = 0xFFFF0000 /*2 a la 32 (4GB) - 2 a la 16 (64KB)*/
    rom (rx) : ORIGIN = 0xFFFF0000, LENGTH = 64K
}

SECTIONS
{
        /* Mapeo de las tablas de sistema en RAM */
        .tablas_sistema __TABLAS_SISTEMA_VMA_LINEAL : AT (__TABLAS_SISTEMA_LMA) {
                *(.tablas_sistema);
        } > ram
        __tablas_sistema_size = SIZEOF(.tablas_sistema);

        __INTERRUPCIONES_LMA = LOADADDR(.tablas_sistema) + SIZEOF(.tablas_sistema);
        /* Mapeo de las interrupciones y teclado en RAM */
        .interrupciones __INTERRUPCIONES_VMA_LINEAL : AT (__INTERRUPCIONES_LMA) {
                *(.interrupciones);
        } > ram
        __interrupciones_size = SIZEOF(.interrupciones);

        __RUTINAS_LMA = LOADADDR(.interrupciones) + SIZEOF(.interrupciones);
        /* Mapeo de las rutinas en RAM */
        .rutinas_ram __RUTINAS_VMA_LINEAL : AT (__RUTINAS_LMA) {
                *(.rutinas);
        } > ram
        __rutinas_size = SIZEOF(.rutinas_ram);

        __TABLA_DIGITOS_LMA = LOADADDR(.rutinas_ram) + SIZEOF(.rutinas_ram);
        /* Mapeo de los datos en RAM */
        .tabla_digitos __TABLA_DIGITOS_VMA_LINEAL : AT (__TABLA_DIGITOS_LMA) {
                *(.tabla_digitos);
        } > ram
        __tabla_digitos_size = SIZEOF(.tabla_digitos);

        __DATOS_LMA = LOADADDR(.tabla_digitos) + SIZEOF(.tabla_digitos);
        /* Mapeo de los datos en RAM */
        .datos __DATOS_VMA_LINEAL : AT (__DATOS_LMA) {
                *(.datos);
                /* *(.data);
                *(.bss); */
                ./bin/pantalla.elf(.note*);
                ./bin/pantalla.elf(.data*);
                ./bin/pantalla.elf(.bss*);

                ./bin/teclado.elf(.note*);
                ./bin/teclado.elf(.data*);
                ./bin/teclado.elf(.bss*);
        } > ram
        __datos_size = SIZEOF(.datos);

        __VIDEO_LMA = LOADADDR(.datos) + SIZEOF(.datos);
        /* Mapeo del video en RAM */
        .video __VIDEO_VMA_LINEAL : AT (__VIDEO_LMA) {
                *(.video);
        } > ram
        __video_size = SIZEOF(.video);

        __KERNEL_LMA = LOADADDR(.video) + SIZEOF(.video);
        /* Mapeo del kernel en RAM*/
        .kernel __KERNEL_VMA_LINEAL : AT (__KERNEL_LMA) {
                *(.kernel);
        } > ram
        __kernel_size = SIZEOF(.kernel);

        /** TAREA 1**/
        __TAREA_1_TEXT_LMA = LOADADDR(.kernel) + SIZEOF(.kernel);
        __TAREA_1_LMA = __TAREA_1_TEXT_LMA;
        /* Mapeo de la tarea 1 en RAM*/
        .tarea_1_text __TAREA_1_TEXT_VMA_LINEAL : AT (__TAREA_1_TEXT_LMA) {
                *(.tarea_1);
                ./bin/tarea_1.elf(.note*);
        } > ram
        __tarea_1_text_size = SIZEOF(.tarea_1_text);

        __TAREA_1_DATA_LMA = LOADADDR(.tarea_1_text) + SIZEOF(.tarea_1_text);
        .tarea_1_data __TAREA_1_DATA_VMA_LINEAL : AT (__TAREA_1_DATA_LMA) {
                ./bin/tarea_1.elf(.data*);
        } > ram
        __tarea_1_data_size = SIZEOF(.tarea_1_data);

        __TAREA_1_RODATA_LMA = LOADADDR(.tarea_1_data) + SIZEOF(.tarea_1_data);
        .tarea_1_rodata __TAREA_1_RODATA_VMA_LINEAL : AT (__TAREA_1_RODATA_LMA) {
                ./bin/tarea_1.elf(.rodata*);
        } > ram
        __tarea_1_rodata_size = SIZEOF(.tarea_1_rodata);

        __TAREA_1_BSS_LMA = LOADADDR(.tarea_1_rodata) + SIZEOF(.tarea_1_rodata);
        .tarea_1_bss __TAREA_1_BSS_VMA_LINEAL : AT (__TAREA_1_BSS_LMA) {
                ./bin/tarea_1.elf(.bss*);
        } > ram
        __tarea_1_bss_size = SIZEOF(.tarea_1_bss);

        __tarea_1_size = SIZEOF(.tarea_1_text) + SIZEOF(.tarea_1_data) + SIZEOF(.tarea_1_rodata) + SIZEOF(.tarea_1_bss);

        /* Mapeo de la ROM */
        __ROM_INIT_LMA = LOADADDR(.tarea_1_bss) + SIZEOF(.tarea_1_bss);
        __ROM_INIT_VMA = __ROM_INIT_LMA;
        .init_rom __ROM_INIT_VMA : AT (__ROM_INIT_LMA) {
                *(.rom_init);
                *(.text);
                *(.rodata);
        } > rom
        __init_rom_size = SIZEOF(.init_rom);

        __RUTINAS_ROM_LMA = LOADADDR(.init_rom) + SIZEOF(.init_rom);
        __RUTINAS_ROM_VMA = __RUTINAS_ROM_LMA;
        /* Mapeo de las funciones auxiliares en ROM */
        .rutinas_rom __RUTINAS_ROM_VMA : AT (__RUTINAS_ROM_LMA) {
                *(.rutinas_rom);
                *(.note.gnu.property);
        } > rom
        __rutinas_rom_size = SIZEOF(.rutinas_rom);

        __TABLAS_SISTEMA_16_LMA = LOADADDR(.rutinas_rom) + SIZEOF(.rutinas_rom);
        __TABLAS_SISTEMA_16_VMA = __TABLAS_SISTEMA_16_LMA;
        /* Mapeo de las tablas del sistema de modo real*/
        .tablas_sistema_16 __TABLAS_SISTEMA_16_VMA : AT (__TABLAS_SISTEMA_16_LMA) {
                *(.tablas_sistema_16);
        } > rom
        __tablas_sistema_16_size = SIZEOF(.tablas_sistema_16);

        __VIDEO_ROM_LMA = LOADADDR(.tablas_sistema_16) + SIZEOF(.tablas_sistema_16);
        __VIDEO_ROM_VMA = __VIDEO_ROM_LMA;
        /* Mapeo de las tablas del sistema de modo real*/
        .video_rom __VIDEO_ROM_VMA : AT (__VIDEO_ROM_LMA) {
                *(.video_rom);
        } > rom
        __video_rom_size = SIZEOF(.video_rom);

        /* Mapeo del reset */
        .reset __RESET_VMA_LINEAL : AT (__RESET_LMA) {
                *(.reset_vector);
        } > rom
        __reset_size = SIZEOF(.reset);

        /* Relleno de la ROM */
        .fill_rom : {
                FILL(0x90);
                . = ORIGIN(rom) + LENGTH(rom)-1;
                BYTE(0x90);
        } > rom
}