#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#define CANTIDAD_HIJOS  5

void sigchld_handler(int sig)
{
    write(0, "Ahhh! SIGCHLD!\n", 15);
    wait(NULL);
}

int main(void)
{
    pid_t pid;
    struct sigaction sa;
    sa.sa_handler = sigchld_handler;
    sa.sa_flags = 0; // or SA_RESTART
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    for(int i = 0; i < CANTIDAD_HIJOS; i++){
        pid = fork();
        switch(pid){
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
                printf("Soy el padre y mi hijo es [PID: %d]. [PID: %d]\n", pid, getpid());
                break;
        }
    }
    
    sleep(30);
    printf("Listo.... [PID: %d]\n", getpid());

    return 0;
}
