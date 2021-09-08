#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#include "../file_manager/manager.h"
int N;
int PIDS1;
int PIDS2;
int PIDS3;
int pid_fabrica;
int* pids_repartidores;
int* estados_semaforos;
int ES1;
int ES2;
int ES3;
int estado_0;
int current_index = 0;
int* T;

/*
 ¡Hola! Bienvenide a nuestro súper código.

 Matías Fernández Carrasco
 Raúl Diaz Campos

 El repo en el que trabajamos:
 https://github.com/mafernandez9/SSOO
 
*/

void handler_fabrica(int sig)
{
  printf("\nCerrando fábrica...\n");
  int i = 0;
  while (i < N)
  {
    if (pids_repartidores[i] != NULL)
    {
      kill(pids_repartidores[i], SIGABRT);
      wait(NULL);
    }
    i++;
  }
  printf("Liberando memoria fabrica\n");
  free(T);
  free(estados_semaforos);
  free(pids_repartidores);

  
  exit(0);
}

void handler_finish(int sig)
{
  printf("Envío de señal para cerrar fábrica pid = %i", pid_fabrica);
  kill(pid_fabrica, SIGABRT);
  wait(NULL);
  int estado = 0;

  kill(PIDS1, SIGABRT);
  kill(PIDS2, SIGABRT);
  kill(PIDS3, SIGABRT);
  printf("\nEnvío de señal para cerrar semáforos pid = [%i, %i, %i] \n", PIDS1, PIDS2, PIDS3);

  printf("Liberando memoria...\n");
  free(T);
  free(estados_semaforos);
  free(pids_repartidores);
  exit(0);
}

void handler_swi(int sig, siginfo_t *siginfo, void *ucontext)
{
  int valor_recibido = siginfo->si_value.sival_int;
  estados_semaforos[valor_recibido] = 1 - estados_semaforos[valor_recibido];
  printf("\n[Fábrica] Ha cambiado el semáforo con id = %i\n", valor_recibido);

  int i = 0;
  while (i < N)
  {
    if (pids_repartidores[i] != NULL)
    {
      send_signal_with_int(pids_repartidores[i], valor_recibido);
    }
    i++;
  }
}

int main(int argc, char const *argv[])
{
  printf("DCCUBER (pid = %i)\n", getpid());
  char *filename;
  if (argc > 1) {
    filename = argv[1];
  } else {
    filename = "input.txt";
  }
  InputFile *data_in = read_file(filename);
  printf("Leyendo el archivo %s...\n", filename);
  printf("- Lineas en archivo: %i\n", data_in->len);
  printf("- Contenido del archivo:\n");
  printf("\t- ");
  for (int i = 0; i < 4; i++)
  {
    printf("%s, ", data_in->lines[0][i]);
  }
  printf("\n");
  printf("\t- ");
  // int N;
  N = atoi(data_in -> lines[1][1]);
  T = calloc(3, sizeof(int));
  int T_c = atoi(data_in -> lines[1][0]);
  T[0] = atoi(data_in -> lines[1][2]);
  T[1] = atoi(data_in -> lines[1][3]);
  T[2] = atoi(data_in -> lines[1][4]);
  int D_BODEGA = atoi(data_in -> lines[0][3]);
  int S1 = atoi(data_in -> lines[0][0]);
  int S2 = atoi(data_in -> lines[0][1]);
  int S3 = atoi(data_in -> lines[0][2]);
  estados_semaforos = calloc(3, sizeof(int));
  estados_semaforos[0] = 1;
  estados_semaforos[1] = 1;
  estados_semaforos[2] = 1;
  for (int i = 0; i < 5; i++)
  {
    printf("%s, ", data_in->lines[1][i]);
  }
  input_file_destroy(data_in);
  pid_fabrica = fork(); //creamos la fabrica

  if (pid_fabrica > 0)
  {
    signal(SIGINT, handler_finish);
    for (int i = 0; i < 3; i++){
      int pids_semaforos = fork ();
      if (pids_semaforos == 0) {
        char id_fabrica[20];
        sprintf(id_fabrica, "%i", pid_fabrica);
        char buffer_time[20];
        sprintf(buffer_time, "%i", T[i]);
        char name[20];
        sprintf(name, "%i", i);
        char *args[] = {"./semaforo", id_fabrica , buffer_time, name, "v", NULL};
        execvp("./semaforo", args);
      }
      if (i == 0)
      {
        PIDS1 = pids_semaforos;
      }
      else if (i == 1)
      {
        PIDS2 = pids_semaforos;
      }
      else if (i == 2)
      {
        PIDS3 = pids_semaforos;
      }
    }
  }
  if (pid_fabrica == 0) {    
    signal(SIGABRT, handler_fabrica);
    connect_sigaction(SIGUSR1, handler_swi);

    printf("\nI'm the FABRICA process and my PID is: %i\n", getpid());
    sleep(T_c);
    pids_repartidores = calloc(N, sizeof(int));
    for (int i = 0; i < N; i++)
      {
        char name[20];
        sprintf(name, "%i", i);
        char dist_bodega[20];
        sprintf(dist_bodega, "%i", D_BODEGA);
        char dist_s1[20];
        sprintf(dist_s1, "%i", S1);
        char dist_s2[20];
        sprintf(dist_s2, "%i", S2);
        char dist_s3[20];
        sprintf(dist_s3, "%i", S3);
        char e_s1[20];
        sprintf(e_s1, "%i", estados_semaforos[0]);
        char e_s2[20];
        sprintf(e_s2, "%i", estados_semaforos[1]);
        char e_s3[20];
        sprintf(e_s3, "%i", estados_semaforos[2]);
        char *args[] = {"./repartidor", name, dist_bodega, dist_s1, dist_s2, dist_s3, e_s1, e_s2, e_s3, NULL};
        pids_repartidores[i] = fork ();
        if (pids_repartidores[i] == 0)
        {
          printf("Enviando a repartir id = %i", pids_repartidores[i]);
          execvp("./repartidor", args);
        }
        sleep(T_c);
      }
      for (int i = 0; i < N; i++) {
        wait(NULL);
        wait(NULL);
      }
  }
  if (pid_fabrica > 0)
  {
        wait(NULL);
  }
  printf("\n");
  if (pid_fabrica > 0)
  {
    printf("Envío de señal para cerrar fabrica pid = %i", pid_fabrica);
    kill(pid_fabrica, SIGABRT);
   
    printf("Envío de señal para cerrar semáforos pid = [%i, %i, %i]", PIDS1, PIDS2, PIDS3);
    kill(PIDS1, SIGABRT);
    kill(PIDS2, SIGABRT);
    kill(PIDS3, SIGABRT);

    printf("Liberando memoria main...");
    free(T);
    free(estados_semaforos);
    free(pids_repartidores);
    exit(0);
  }
  if (pid_fabrica == 0) {
    printf("Liberando memoria fabrica...");
    free(T);
    free(estados_semaforos);
    free(pids_repartidores);
    kill(getpid(), SIGABRT);
  }

  // exit(0);
}
