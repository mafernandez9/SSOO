#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>

#include "../file_manager/manager.h"

int name;
int id_fabrica;
int contador = 0;
int t=0;

void handler(int sig)
{
  int estado_0 = 0;

  printf("\nbotaron al semaforo %i, tiempo -> %i\n", name, t);
  char semaforo[20];
  sprintf(semaforo, "semaforo_%i.txt", name+1);
  FILE *output = fopen(semaforo, "w");

  fprintf(output, "%i", contador);
  // No agregamos el separador al último número
 

  // Se cierra el archivo (si no hay leak)
  fclose(output);

  // Terminamos el programa con exit code 0
  
  exit(0);
}

int main(int argc, char const *argv[])
{
  signal(SIGABRT, handler);
  printf("I'm the SEMAFORO process and my PID is: %i pid_fabrica -> %s , delay -> %s , nombre -> %s , state -> %s \n", getpid(), argv[1], argv[2], argv[3], argv[4]);
  id_fabrica = atoi(argv[1]);
  printf("%i", id_fabrica);
  int tiempo = atoi(argv[2]);
  name = atoi(argv[3]);
  int estado = 1; //1 -> verde, 0 -> rojo
  
  while(t < 30)
  {
    t++;
    if (t % tiempo == 0)
    {
      if (estado == 1)
      {
        printf("soy el semaforo %i y mi estado es %i\n", name, estado);
        estado = 0;
        contador++;
        send_signal_with_int(id_fabrica, name);
      }
      else if (estado == 0)
      {
        printf("soy el semaforo %i y mi estado es %i\n", name, estado);
        estado = 1;
        contador++;
        send_signal_with_int(id_fabrica, name);
      }
    }
    sleep(1);
    // t++;
    
  }
  // wait(NULL);
  kill(getpid(), SIGABRT);
}
