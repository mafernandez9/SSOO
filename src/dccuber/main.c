#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

#include "../file_manager/manager.h"

void handler(int sig, siginfo_t *siginfo, void *ucontext)
{
  int valor_recibido = siginfo->si_value.sival_int;
  if (valor_recibido == -1)
  {
    
  }
}

int main(int argc, char const *argv[])
{
  printf("I'm the DCCUBER process and my PID is: %i\n", getpid());
  char *filename = "input.txt";
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
  int N;
  N = atoi(data_in -> lines[1][1]);
  int* T = calloc(3, sizeof(int));
  int T_c = atoi(data_in -> lines[1][0]);
  T[0] = atoi(data_in -> lines[1][2]);
  T[1] = atoi(data_in -> lines[1][3]);
  T[2] = atoi(data_in -> lines[1][4]);
  int D_BODEGA = atoi(data_in -> lines[0][3]);
  for (int i = 0; i < 5; i++)
  {
    printf("%s, ", data_in->lines[1][i]);
  }
  int pid_fabrica = fork(); //creamos la fabrica

  if (pid_fabrica > 0)
  {
    int usable = 1;
    for (int i = 0; i < 3; i++){
      int pids_semaforos = fork ();
      if (pids_semaforos == 0) {
        usable = 0;
        char id_fabrica[20];
        sprintf(id_fabrica, "%i", pid_fabrica);
        char buffer_time[20];
        sprintf(buffer_time, "%i", T[i]);
        char name[20];
        sprintf(name, "%i", i);
        char *args[] = {"./semaforo", id_fabrica , buffer_time, name, "v", NULL};
        execvp("./semaforo", args);
      }
     if (usable > 0) {
        wait(NULL);
      }
    }
  }
  if (pid_fabrica == 0) {
    printf("\nI'm the FABRICA process and my PID is: %i\n", getpid());
    sleep(2);
    int* pids_repartidores = calloc(N, sizeof(int));
    for (int i = 0; i < N; i++)
      {
        char name[20];
        sprintf(name, "%i", i);
        char dist_bodega[20];
        sprintf(dist_bodega, "%i", D_BODEGA);
        char *args[] = {"./repartidor", name, dist_bodega, NULL};
        pids_repartidores[i] = fork ();
        if (pids_repartidores[i] > 0)
        {
          execvp("./repartidor", args);
        }
        sleep(T_c);
      }
  }
  if (pid_fabrica > 0)
  {
        wait(NULL);
  }
  printf("\n");
  printf("Liberando memoria...\n");
  input_file_destroy(data_in);
}
