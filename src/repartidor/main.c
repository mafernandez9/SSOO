#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>

#include "../file_manager/manager.h"

int* estados_semaforos;


void handler(int sig)
{
  printf("\nYA Q ONDA DEJAME REPARTIR TRANQUILE %i\n", getpid());
  exit(0);
}

void handler_signals(int sig, siginfo_t *siginfo, void *ucontext)
{
  int valor_recibido = siginfo->si_value.sival_int;
  printf("\nEstoy entrando, semaforo numero %i\n", valor_recibido);
  estados_semaforos[valor_recibido] = 1 - estados_semaforos[valor_recibido];
} 

int main(int argc, char const *argv[])
{
  signal(SIGABRT, handler);
  connect_sigaction(SIGUSR1, handler_signals);
  printf("I'm the REPARTIDOR process and my PID is: %i, my id is -> %s and the distance to b is -> %s , to s1 is -> %s , to s2 is -> %s, to s3 is -> %s\n", getpid(), argv[1], argv[2], argv[3], argv[4], argv[5]);
  int posicion = 0;
  int id = atoi(argv[1]);
  int p_bodega = atoi(argv[2]);
  estados_semaforos = calloc(3, sizeof(int));
  estados_semaforos[0] = atoi(argv[6]);
  estados_semaforos[1] = atoi(argv[7]);
  estados_semaforos[2] = atoi(argv[8]);
  while (posicion < p_bodega)
  {
    posicion++;
    printf("my pid is %i and my position is %i\n", getpid(), posicion);
    sleep(1);
  }
  wait(NULL);
  kill(getpid(), SIGABRT);
}
