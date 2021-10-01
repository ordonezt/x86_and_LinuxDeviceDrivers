#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <pthread.h>

#include "../../inc/servidor/servidor.h"
#include "../../inc/comunes/config.h"
#include "../../inc/comunes/ipc/sockets.h"
#include "../../inc/comunes/collections/list.h"

/**
 * @brief Aplicacion horrible que genera hijos (y zombies) a lo loco para probar el servidor.
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, char argv[])
{
    int socket;
    struct sockaddr_in info_socket;

    while(1)
    {
        if(fork() == 0)
        {
            socket = crear_socket(PF_INET, SOCK_STREAM, 0, 5000, &info_socket, "127.0.0.1", 0);
            if(socket <= 0)
            {
                printf("No me pude conectar!\n");
                return 1;
            }
            printf("Me conecte!\n");
        }
        sleep(1);
    }
    return 0;
}