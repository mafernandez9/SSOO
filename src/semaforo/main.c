#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "../file_manager/manager.h"

void handler(int sig)
{
  exit(0);
}

int main(int argc, char const *argv[])
{
  signal(SIGABRT, handler);
  printf("I'm the SEMAFORO process and my PID is: %i pid_fabrica -> %s , delay -> %s , nombre -> %s , state -> %s \n", getpid(), argv[1], argv[2], argv[3], argv[4]);
  int id_fabrica = atoi(argv[1]);
  printf("%i", id_fabrica);
  int tiempo = atoi(argv[2]);
  int name = atoi(argv[3]);
  int estado = 1; //1 -> verde, 0 -> rojo
  int t=0;
  while(t < 20)
  {
    if (t % tiempo == 0)
    {
      if (estado == 1)
      {
        printf("soy el semaforo %i y mi estado es %i\n", name, estado);
        estado = 0;
        send_signal_with_int(id_fabrica, name);
      }
      else if (estado == 0)
      {
        printf("soy el semaforo %i y mi estado es %i\n", name, estado);
        estado = 1;
        send_signal_with_int(id_fabrica, name);
      }
    }
    sleep(1);
    t++;
  }
  wait(NULL);
}
