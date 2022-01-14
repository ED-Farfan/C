// compile with $ gcc -Wall -g *.c -pthread -o program
// run with ./program
// check with valgrind --tool=helgrind ./program
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void* thread_run(void* data)
{ sleep(2); 
  printf("[TH_1:%ld]: Hola desde hilo \n", pthread_self());
  sleep(1);
  (*(int*)data)++;
  printf("[TH_1: %ld]: Salir...............\n",pthread_self());
  pthread_exit(data);
}

int main()
{
  pthread_t thread;
  int data=0;
  int thread_rc;
  printf("[MAIN:%ld]: INICIAMOS............ \n",pthread_self());
  if ((thread_rc=pthread_create(&thread,NULL,thread_run,&data))!=0)
  {
    printf("Error al crear el hilo. Code %i",thread_rc);
    return -1;
  }
  sleep(1);
  printf("[MAIN:%ld]: Thread allocated \n",pthread_self());
  int *ptr_output_data;
  pthread_join(thread,(void **)&ptr_output_data);
  printf("[MAIN:%ld]: Hilo Retorno %d \n",pthread_self(), *ptr_output_data);
  return 0;
} 