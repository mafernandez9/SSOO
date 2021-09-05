#include <stdio.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
  printf("I'm the SEMAFORO process and my PID is: %i pid_fabrica -> %s , delay -> %s , nombre -> %s , state -> %s \n", getpid(), argv[1], argv[2], argv[3], argv[4]);
  int id_fabrica = atoi(argv[1]);
  int tiempo = atoi(argv[2]);
  int name = atoi(argv[3]);
  while ()
  }
