#ifndef __SIGNAL_H
#define __SIGNAL_H

#include <signal.h>

typedef struct{
    struct sigaction sig_action;
    int n_signal;
}signal_t;

/**
 * @brief Define el manejador (handler) de una señal.
 * 
 * @param signal Estructura de control de la señal.
 * @param handler Manejador a utilizar.
 * @param n_signal Numero de la señal a atrapar.
 * @return int 0 exito, -1 error
 */
int atrapar_signal(signal_t *signal, void(*handler)(int sig), int n_signal);

#endif /*__SIGNAL_H*/