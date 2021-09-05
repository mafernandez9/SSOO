#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char const *argv[])
{
  printf("I'm the REPARTIDOR process and my PID is: %i, my id is -> %s and the distance is -> %s\n", getpid(), argv[1], argv[2]);
  int posicion = 0;
  int id = atoi(argv[1]);
  int p_bodega = atoi(argv[2]);
  while (posicion < p_bodega)
  {
    posicion++;
    printf("my pid is %i and my position is %i\n", getpid(), posicion);
    sleep(1);
  }
  wait(NULL);
}
