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
// int numbers[9] = {1, 2, 3, 4, 5, 6, 7, 125, 9};

void handler_fabrica(int sig)
{
  printf("\nCerraré la fábrica\n");
  int i = 0;
  printf("\n N = %i \n", N);
  while (i < N)
  {
    printf("\nchao repartidor %i\n", i);
    if (pids_repartidores[i] != NULL)
    {
      printf("\nNO ES NULL AAAAAAAA %i\n", i);
      kill(pids_repartidores[i], SIGABRT);
      wait(NULL);
    }
    i++;
  }
  // wait(NULL);
  printf("CHAO NO VIMO FABRIK");
  // Abrimos un archivo en modo de lectura


  
  exit(0);
}

void handler_finish(int sig)
{
  printf("\nHola, Raúl eliminará las canciones de Ñengo Flow\n");
  printf("\n pids -> [%i, %i, %i], pid_fabrica -> %i\n", PIDS1, PIDS2, PIDS3, pid_fabrica);
  kill(pid_fabrica, SIGABRT);
  wait(NULL);
  int estado = 0;
  // https://stackoverflow.com/questions/21248840/example-of-waitpid-in-use
  // waitpid(pid_fabrica, &estado_0, WUNTRACED || WCONTINUED);

  kill(PIDS1, SIGABRT);
  kill(PIDS2, SIGABRT);
  kill(PIDS3, SIGABRT);
  printf("\nEl pid de la fabrica es: %i \n", pid_fabrica);
  waitpid(PIDS1, &estado_0, WUNTRACED || WCONTINUED);
  waitpid(PIDS2, &estado_0, WUNTRACED || WCONTINUED);
  waitpid(PIDS3, &estado_0, WUNTRACED || WCONTINUED);
  exit(0);
}

void handler_swi(int sig, siginfo_t *siginfo, void *ucontext)
{
  printf("\n ola soy la fábrica y un semaforo cambió\n");
  int valor_recibido = siginfo->si_value.sival_int;
  estados_semaforos[valor_recibido] = 1 - estados_semaforos[valor_recibido];
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
  // int N;
  N = atoi(data_in -> lines[1][1]);
  int* T = calloc(3, sizeof(int));
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
    printf("\nConectando handler fábrica\n");
    
    signal(SIGABRT, handler_fabrica);
    
    printf("\nConectando handler semaforos fábrica\n");
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
  if (pid_fabrica > 0)
  {
    // printf("\nHola, Raúl eliminará las canciones de Ñengo Flow\n");
    printf("\n pids -> [%i, %i, %i], pid_fabrica -> %i\n", PIDS1, PIDS2, PIDS3, pid_fabrica);
    kill(pid_fabrica, SIGABRT);
    // int estado = 0;
    // https://stackoverflow.com/questions/21248840/example-of-waitpid-in-use
    // waitpid(pid_fabrica, &estado_0, WUNTRACED || WCONTINUED);

    kill(PIDS1, SIGABRT);
    kill(PIDS2, SIGABRT);
    kill(PIDS3, SIGABRT);
    printf("\nEl pid de la fabrica es: %i \n", pid_fabrica);
    // waitpid(PIDS1, &estado_0, WUNTRACED || WCONTINUED);
    // waitpid(PIDS2, &estado_0, WUNTRACED || WCONTINUED);
    // waitpid(PIDS3, &estado_0, WUNTRACED || WCONTINUED);
    exit(0);
  }

  exit(0);
}
