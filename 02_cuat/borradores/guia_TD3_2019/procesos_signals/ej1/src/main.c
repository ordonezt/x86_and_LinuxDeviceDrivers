#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
    pid_t pid;
    int wstatus;

    pid = fork();
    switch(pid){
        case -1:
            printf("Error. [PID: %d]\n", getpid());
            break;
        
        case 0:
            printf("Soy el proceso hijo y mi padre es [PID: %d]!. [PID: %d]\n", getppid(), getpid());
            printf("Me voy a dormir. [PID: %d]\n", getpid());
            sleep(2);
            printf("Me muero. [PID: %d]\n", getpid());
            exit(0);
            break;
        
        default:
            printf("Soy el padre y mi hijo es [PID: %d]. [PID: %d]\n", pid, getpid());
            printf("Esperando q se muera mi hijo. [PID: %d]\n", getpid());
            wait(&wstatus);
            printf("Listo.... [PID: %d]\n", getpid());
            break;
    }
    
    return 0;
}
