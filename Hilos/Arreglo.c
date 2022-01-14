// compile with $ gcc -Wall -g Arreglo.c -pthread -o program
// run with ./program
// check with valgrind --tool=helgrind ./program
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#define Numero 10
void *thread_run(void *data)
{
    sleep(2);
    int *x = (int*)data;
    printf("[TH_%d:%ld]: Hola desde hilo \n",*x, pthread_self());
    sleep(1);
    (*(int *)data)++;
    printf("[TH_%d: %ld]: Salir...............\n",*x, pthread_self());
    pthread_exit(data);
}

int main()
{
    pthread_t thread[Numero];
    int data[Numero];
    int thread_rc;
    printf("[MAIN:%ld]: INICIAMOS............ \n", pthread_self());
    for (register int i = 0; i < Numero; i++)
    {
        data[i]=i;
        if ((thread_rc = pthread_create(&thread[i], NULL, thread_run, &data[i])) != 0)
        {
            printf("Error al crear el hilo. Code %i", thread_rc);
            return -1;
        }
    }

    sleep(1);
    printf("[MAIN:%ld]: Thread allocated \n", pthread_self());
    int *ptr_output_data;
    for (register int i = 0; i < Numero; i++)
    {
        pthread_join(thread[i], (void **)&ptr_output_data);
        printf("[MAIN:%ld]: Hilo Retorno %d \n", pthread_self(), *ptr_output_data);
    }    
    return 0;
}