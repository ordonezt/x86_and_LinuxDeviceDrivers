#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#include "../inc/main.h"


volatile sig_atomic_t flag_crear_hijo, flag_hijo_muerto;

void sigusr1_handler(int sig)
{
    flag_crear_hijo = 1;
}

void sigchld_handler(int sig)
{
    flag_hijo_muerto = 1;
}

int main(void)
{
    struct sigaction sa_usr1, sa_chld;
    pid_t pid, pids_vec[CANTIDAD_HIJOS];
    lista_hijos_t lista;
    
    lista.longitud = CANTIDAD_HIJOS;
    lista.p = pids_vec;

    printf("Hola, soy el proceso [PID %d] y recien arranco mi ejecucion\n", getpid());

    flag_crear_hijo = 0;
    sa_usr1.sa_handler = sigusr1_handler;
    sa_usr1.sa_flags = 0; // or SA_RESTART
    sigemptyset(&sa_usr1.sa_mask);
    if (sigaction(SIGUSR1, &sa_usr1, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    flag_hijo_muerto = 0;
    sa_chld.sa_handler = sigchld_handler;
    sa_chld.sa_flags = 0; // or SA_RESTART
    sigemptyset(&sa_chld.sa_mask);
    if (sigaction(SIGCHLD, &sa_chld, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }


    while(1){
        printf("Hijos vivos %d\n", CANTIDAD_HIJOS - lista_hijos_vacios(&lista));

        if(flag_hijo_muerto){
            flag_hijo_muerto = 0;
            esperar_hijos(&lista);
            printf("Hijos vivos %d\n", CANTIDAD_HIJOS - lista_hijos_vacios(&lista));
        }

        if(flag_crear_hijo && (lista_hijos_vacios(&lista) != 0)){
            flag_crear_hijo = 0;
            pid = fork();
            switch(pid){
                case -1:
                    printf("Error en fork\n");
                break;

                case 0:
                    //Proceso hijo
                    sleep(40);
                    exit(0);
                break;
                
                default:
                    //Proceso padre
                    printf("Tuve un hijo [PID: %d]\n", pid);
                    lista_hijos_agregar(&lista, pid);
                break;
            }
        }
        sleep(5);
    }

    return 0;
}

int lista_hijos_vacios(lista_hijos_t *lista){
    int cuenta=0;
    for(int i=0; i<lista->longitud; i++){
        if(lista->p[i] != 0) cuenta++;
    }

    return lista->longitud - cuenta;
}

int lista_hijos_agregar(lista_hijos_t *lista, pid_t pid){
    if(lista_hijos_vacios(lista) != 0){
        for(int i=0; i<lista->longitud; i++){
            if(lista->p[i] == 0){
                lista->p[i] = pid;   
                return 0;
            }
        }
    }

    return -1;
}

int esperar_hijos(lista_hijos_t *lista){
    int cuenta = 0;
    for(int i=0; i<lista->longitud; i++){
        if(lista->p[i] != 0){
            if(waitpid(lista->p[i], NULL, WNOHANG) > 0){
                lista->p[i] = 0;
                cuenta++;
            }
        }
    }
    return cuenta;
}