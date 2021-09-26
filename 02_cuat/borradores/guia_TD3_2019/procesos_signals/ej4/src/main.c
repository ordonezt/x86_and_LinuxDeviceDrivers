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

void sigchld_handler(int sig)
{
    for(int i = 0; i < CANTIDAD_HIJOS; i++){
        if(pids_vec[i] != 0){
            write(0, "Esperando que muera un hijo\n", 29);
            switch(waitpid(pids_vec[i], NULL, WNOHANG))
            {
                case -1:
                    write(0, "Error waitpid\n", 14);
                break;
                case 0:
                    write(0, "Todavia no murio\n", 18);
                    break;
                default:
                    pids_vec[i] = 0;
                    write(0, "MURIO\n", 6);
                break;
            }
        }
    }
}

int main(void)
{
    struct sigaction sa;
    sa.sa_handler = sigchld_handler;
    sa.sa_flags = 0; // or SA_RESTART
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    for(int i = 0; i < CANTIDAD_HIJOS; i++){
        pids_vec[i] = fork();
        switch(pids_vec[i]){
            case -1:
                printf("Error. [PID: %d]\n", getpid());
                return 1;
                break;
            
            case 0:
                printf("Soy el proceso hijo y mi padre es [PID: %d]!. [PID: %d]\n", getppid(), getpid());
                sleep(5);
                printf("Me muero. [PID: %d]\n", getpid());
                exit(0);
                break;
            
            default:
                printf("Soy el padre y mi hijo es [PID: %d]. [PID: %d]\n", pids_vec[i], getpid());
                break;
        }
    }
    
    while(1){
    sleep(40);
    printf("Listo.... [PID: %d]\n", getpid());}

    return 0;
}
