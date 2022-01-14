#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/shm.h>
#include <sys/ipc.h>

#define SEM_PRODUCER_NAME "/myproducer"
#define SEM_CONSUMER_NAME "/myconsumer"
#define SEM_PRODUCER_NAME2 "/myproducer2"
#define SEM_CONSUMER_NAME2 "/myconsumer2"

typedef struct
{
    int id;
    int inicio;
    int Final;
} Productor;
typedef struct
{
    int id;
    int repeticiones;
} Consumidor;

sem_t *sem_prod1 = NULL;
sem_t *sem_cons1 = NULL;
sem_t *sem_prod2 = NULL;
sem_t *sem_cons2 = NULL;

int varCrit[2], n;

void *producer(void *args)
{

    //Rangos a producir
    Productor *pr = (Productor *)args;
    int inicio = pr->inicio;
    int fin = pr->Final;

    for (int i = inicio; i < fin; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            if (j == 0)
            {
                sem_wait(sem_prod1);
                varCrit[i] = 2 * i; //<-esto representa la seccion critica 1
                printf("\nProductor[%d] en seccion 1: %d\n", pr->id, varCrit[i]);
                sem_post(sem_cons1);
            }
            else
            {
                sem_wait(sem_prod2);
                varCrit[i] = 2 * i + 1; //<-esto representa la seccion critica 1
                printf("\nProductor[%d] en seccion 2: %d\n", pr->id, varCrit[i]);
                sem_post(sem_cons2);
            }
        }
    }
    printf("Termino Productor[%d] \n", pr->id);
}

void *consumer(void *args)
{
    Consumidor *cns = (Consumidor *)args;
    int r = cns->repeticiones;
    for (int i = 0; i < r; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            if (j == 0)
            {
                sem_wait(sem_cons1);
                printf("\nconsumiendor[%d] en seccion 1: %d\n", cns->id, varCrit[i]);
                sem_post(sem_prod1);
            }
            else
            {
                sem_wait(sem_cons2);
                printf("\nconsumiendor[%d] en seccion 2: %d\n", cns->id, varCrit[i]);
                sem_post(sem_prod2);
            }
        }
    }
    printf("Termino Consumidor[%d] \n", cns->id);
}

int main(int argc, char *argv[])
{

    int nprod = atoi(argv[1]);
    int ncons = atoi(argv[2]);
    n = atoi(argv[3]);
    int semaforoP = -1, semforoC = -1,semaforoP2 = -1, semforoC2 = -1;

    /*Estos dos sem son para la primera seccion critica*/

    sem_prod1 = sem_open(SEM_PRODUCER_NAME, O_CREAT, S_IRUSR | S_IWUSR | S_IWGRP, 0);
    sem_cons1 = sem_open(SEM_CONSUMER_NAME, O_CREAT, S_IRUSR | S_IWUSR | S_IWGRP, 0);
    sem_prod2 = sem_open(SEM_PRODUCER_NAME2, O_CREAT, S_IRUSR | S_IWUSR | S_IWGRP, 0);
    sem_cons2 = sem_open(SEM_CONSUMER_NAME2, O_CREAT, S_IRUSR | S_IWUSR | S_IWGRP, 0);

    sem_getvalue(sem_prod1, &semaforoP);
    sem_getvalue(sem_cons1, &semforoC);
    sem_getvalue(sem_prod2, &semaforoP2);
    sem_getvalue(sem_cons2, &semforoC2);
    printf("\n1.-Valor del semaforo Productor1[%d] Consumidor1[%d] Productor2[%d] Consumidor2[%d]\n", semaforoP, semforoC,semaforoP2, semforoC2);
    while (semaforoP > 0)
    {
        printf("\nHola\n");
        sem_wait(sem_prod1);
        sem_getvalue(sem_prod1, &semaforoP);
        
    }
    while (semforoC > 0)
    {
        printf("\nAdios\n");
        sem_wait(sem_cons1);
        sem_getvalue(sem_cons1, &semforoC);        
    }
    while (semaforoP2 > 0)
    {
        printf("\nHola xD\n");
        sem_wait(sem_prod2);
        sem_getvalue(sem_prod2, &semaforoP2);
        
    }
    while (semforoC2 > 0)
    {
        printf("\nAdios xD\n");
        sem_wait(sem_cons2);
        sem_getvalue(sem_cons2, &semforoC2);        
    }

    printf("\n->Valor del semaforo Productor1[%d] Consumidor1[%d] Productor2[%d] Consumidor2[%d]\n", semaforoP, semforoC,semaforoP2, semforoC2);

    if (sem_prod1 == SEM_FAILED)
    {
        printf("Error al crear el semáforo\n");
        exit(-1);
    }

    if (sem_cons1 == SEM_FAILED)
    {
        printf("Error al crear el semáforo\n");
        exit(-1);
    }

    pthread_t *prodth = NULL, *consth = NULL;
    prodth = (pthread_t *)malloc(sizeof(pthread_t *) * nprod);
    consth = (pthread_t *)malloc(sizeof(pthread_t *) * ncons);

    Productor *Productors;
    Productors = (Productor *)malloc(sizeof(Productor) * nprod);

    Consumidor *Consumidors;
    Consumidors = (Consumidor *)malloc(sizeof(Consumidor) * ncons);

    //Parametros para definir el numero de elementos a producir por cada productor
    int TodosP = n / nprod, ExtraP = n % nprod;
    int Inicio = 0;
    //Asignamos a los hilos productores

    for (int i = 0; i < nprod; i++)
    {
        Productors[i].id = i + 1;
        Productors[i].inicio = Inicio;
        Productors[i].Final = Inicio + (ExtraP > 0 ? TodosP + 1 : TodosP);
        Inicio = Productors[i].Final;
        ExtraP -= (ExtraP > 0 ? 1 : 0);
        printf("\nPoductor[%d][%d,%d)\n", Productors[i].id, Productors[i].inicio, Productors[i].Final);
        pthread_create(&prodth[i], NULL, (void *)producer, (void *)&Productors[i]);
    }

    //asignamos a los hilos consumidor
    int TodosC = n / ncons, ExtraC = n % ncons;
    for (int i = 0; i < ncons; i++)
    {
        Consumidors[i].id = i + 1;
        Consumidors[i].repeticiones = (ExtraC > 0 ? TodosC + 1 : TodosC);
        ExtraC -= (ExtraC > 0 ? 1 : 0);
        //printf("\n Consumidores[%d] Repeticiones[%d] \n",Consumidors[i].id,Consumidors[i].repeticiones);
        pthread_create(&consth[i], NULL, (void *)consumer, (void *)&Consumidors[i]);
    }

    sem_post(sem_prod1);
    sem_post(sem_prod2);
    //Esperar a los hilos productores
    for (int i = 0; i < nprod; i++)
    {
        pthread_join(prodth[i], NULL);
    }
    //Esperar a los hilos Consumidores
    for (int i = 0; i < ncons; i++)
    {
        pthread_join(consth[i], NULL);
    }
    sem_close(sem_prod1);
    sem_close(sem_cons1);
    sem_close(sem_prod2);
    sem_close(sem_cons2);

    return 0;
}
//gcc semaforos.c  -lpthread -o semaforos_cruzados