#include <stdio.h>

extern void my_itoa(char *buf, int pid);
extern int my_strlen(char *buf);
extern char * my_strrev(char *buf);

/* int main(void)
{
    char buffer[5]; //Lo relleno de cero para evitarme algun quilombo en asm por el momento
    int largo, pid = 1234;

    my_itoa(buffer, 123);
    largo = my_strlen(buffer);

    printf("El PID es: %s\n", buffer);
    printf("Tiene un largo de %d %s\n", largo, buffer);
    printf("Al revez es: %s\n", my_strrev(buffer));
    return 0;
} */