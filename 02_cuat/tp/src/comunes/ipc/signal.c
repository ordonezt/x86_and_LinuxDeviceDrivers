#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include "../../../inc/comunes/ipc/signal.h"

/**
 * @brief Define el manejador (handler) de una señal.
 * 
 * @param signal Estructura de control de la señal.
 * @param handler Manejador a utilizar.
 * @param n_signal Numero de la señal a atrapar.
 * @return int 0 exito, -1 error
 */
int atrapar_signal(signal_t *signal, void(*handler)(int sig), int n_signal){
    signal->sig_action.sa_handler = handler;
    signal->sig_action.sa_flags = 0; // or SA_RESTART
    signal->n_signal = n_signal;
    
    if(sigemptyset(&signal->sig_action.sa_mask)){
        perror("sigemptyset\n");
        return -1;
    }
    
    if (sigaction(n_signal, &signal->sig_action, NULL) == -1){
        perror("sigaction\n");
        return -1;
    }

    return 0;
}