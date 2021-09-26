#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#define CANTIDAD_HIJOS  5

pid_t pids_vec[CANTIDAD_HIJOS];

void sigterm_handler(int sig)
{
    printf("Señal SIGTERM interceptada\n");
    //write(0, "Señal SIGTERM interceptada\n", 28);
}

int main(void)
{
    struct sigaction sa;
    char string[] = "Hola Mundo!";

    printf("Hola, soy el proceso [PID %d] y recien arranco mi ejecucion\n", getpid());

    sa.sa_handler = sigterm_handler;
    sa.sa_flags = 0; // or SA_RESTART

    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGTERM, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    free(string);
    
    while(1){
        printf("Hola Mundo! [PID %d]\n", getpid());
        sleep(5);
    }

    return 0;
}