#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>

#include "../file_manager/manager.h"

int* estados_semaforos;
int* contadores;
int contador_bodega = 0;
int* dist_semaforos;
int t;
int name;


void handler(int sig)
{
  printf("\nYA Q ONDA DEJAME REPARTIR TRANQUILE %i\n", getpid());
  char repartidor[200];
  sprintf(repartidor, "repartidor_%i.txt", name);
  FILE *output = fopen(repartidor, "w");

  fprintf(output, "%i,%i,%i,%i", contadores[0], contadores[1], contadores[2], contador_bodega);
  fclose(output);
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
  t = 0;
  name = atoi(argv[1]);
  int p_bodega = atoi(argv[2]);
  estados_semaforos = calloc(3, sizeof(int));
  contadores = calloc(3, sizeof(int));
  dist_semaforos = calloc(3, sizeof(int));
  dist_semaforos[0] = atoi(argv[3]);
  dist_semaforos[1] = atoi(argv[4]);
  dist_semaforos[2] = atoi(argv[5]);
  contadores[0] = -1;
  contadores[1] = -1;
  contadores[2] = -1;
  contador_bodega = -1;
  int seguir = 1;

  estados_semaforos[0] = atoi(argv[6]);
  estados_semaforos[1] = atoi(argv[7]);
  estados_semaforos[2] = atoi(argv[8]);
  
  while (posicion < p_bodega)
  {
    if (seguir == 1) {
      posicion++;
    }
    seguir = 1;
    printf("my pid is %i and my position is %i\n", getpid(), posicion);
    for (int i = 0; i < 3; i++)
    {
      if (dist_semaforos[i] == posicion) {
        contadores[i] = t;
      }
       else if (dist_semaforos[i] == posicion + 1) {
         if (estados_semaforos[i] == 0) {
           printf("\nRepartidor %i esperando a semaforo %i \n", name, i+1);
           seguir = 0;
         }
      }
    }
    if (p_bodega == posicion) {
        contador_bodega = t;
        kill(getpid(), SIGABRT);
    }
    sleep(1);
    t++;
  }
  wait(NULL);
  kill(getpid(), SIGABRT);
}
