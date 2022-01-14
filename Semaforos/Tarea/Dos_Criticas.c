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
#define SEM_PRODUCER_NAMEG "/myproducerG"
#define SEM_CONSUMER_NAMEG "/myconsumerG"

int varCrit[2];
int Terminar = 0;
int n=0,m=0;
sem_t *sem_prod1 = NULL;
sem_t *sem_cons1 = NULL;
sem_t *sem_prod2 = NULL;
sem_t *sem_cons2 = NULL;
sem_t *sem_prodG = NULL;
sem_t *sem_consG = NULL;

typedef struct
{
    int id;
    int inicio;
    int Final;
    int Termine;
} Productor;
typedef struct
{
    int id;    
} Consumidor;

void *productor(void *args)
{

    //Rangos a producir
    Productor *pr = (Productor *)args;
    int inicio = pr->inicio;
    int fin = pr->Final;
    int sem = 0;
    for (int i = inicio; i < fin; i++)
    {
        sem_wait(sem_prodG);
        //printf("\nProductor[%d] en Critica 1: \n", pr->id);
        if (sem_trywait(sem_prod1) == 0)
        {

            varCrit[0] = i; //<-esto representa la seccion critica 1
            printf("\nProductor[%d] en seccion 1: %d\n", pr->id, varCrit[0]);
            sem_post(sem_cons1);
            sem_post(sem_consG);
        }
        else
        {
            
            sem_wait(sem_prod2);
            varCrit[1] = i; //<-esto representa la seccion critica 1
            printf("\nProductor[%d] en seccion 2: %d\n", pr->id, varCrit[1]);
            sem_post(sem_cons2);
            sem_post(sem_consG);
        }
        
    }
    printf("Termino Productor[%d] \n", pr->id);
    pr->Termine = 1;
}
void *Verificar( Productor ** raiz){
    Productor * r =*raiz;
    while (1==1)
    {
        int u=0;
        for(int i=0; i < m; i++){                
            u+= r[i].Termine;
        }
        if(u==m){
            sleep(1);            
            Terminar = -1;
            break;
        }
    }
    
}

void *consumidor(void *args)
{
    Consumidor *cns = (Consumidor *)args;
    int sem = 0;    
    while (1 == 1){  
     
        if (sem_trywait(sem_consG) == 0)
        {   //printf("\nConsumidor[%d] en seccion \n", cns->id);
            if (sem_trywait(sem_cons1) == 0)
            {

                
                printf("\nConsumidor[%d] en seccion 1: %d\n", cns->id, varCrit[0]);
                sem_post(sem_prod1);
            }
            else
            {
                sem_wait(sem_cons2);                
                printf("\nConsumidor[%d] en seccion 2: %d\n", cns->id, varCrit[1]);
                sem_post(sem_prod2);
            }
            sem_post(sem_prodG);
        }else if (Terminar == -1)
        {
            break;
        }        
    }
    printf("Termino Consumidor[%d] \n", cns->id);
}
int main(int argc, char *argv[])
{

    int nprod = atoi(argv[1]); // Numero de productores
    int ncons = atoi(argv[2]); // Numero de consumidores
    m=nprod;
    n = atoi(argv[3]); // Producciones
    int semaforoP = -1, semforoC = -1, semaforoP2 = -1, semforoC2 = -1, semaforoPG = -1, semforoCG = -1;

    sem_prod1 = sem_open(SEM_PRODUCER_NAME, O_CREAT, S_IRUSR | S_IWUSR | S_IWGRP, 0);
    sem_cons1 = sem_open(SEM_CONSUMER_NAME, O_CREAT, S_IRUSR | S_IWUSR | S_IWGRP, 0);
    sem_prod2 = sem_open(SEM_PRODUCER_NAME2, O_CREAT, S_IRUSR | S_IWUSR | S_IWGRP, 0);
    sem_cons2 = sem_open(SEM_CONSUMER_NAME2, O_CREAT, S_IRUSR | S_IWUSR | S_IWGRP, 0);
    sem_prodG = sem_open(SEM_PRODUCER_NAMEG, O_CREAT, S_IRUSR | S_IWUSR | S_IWGRP, 2);
    sem_consG = sem_open(SEM_CONSUMER_NAMEG, O_CREAT, S_IRUSR | S_IWUSR | S_IWGRP, 0);

    sem_getvalue(sem_prod1, &semaforoP);
    sem_getvalue(sem_cons1, &semforoC);
    sem_getvalue(sem_prod2, &semaforoP2);
    sem_getvalue(sem_cons2, &semforoC2);
    sem_getvalue(sem_prod2, &semaforoPG);
    sem_getvalue(sem_cons2, &semforoCG);
    printf("\n1.-Valor del semaforo Productor1[%d] Consumidor1[%d] Productor2[%d] Consumidor2[%d] ProductorG[%d] ConsumidorG[%d]\n", semaforoP, semforoC, semaforoP2, semforoC2, semaforoPG, semforoCG);
    while (semaforoP > 0)
    {
        sem_wait(sem_prod1);
        sem_getvalue(sem_prod1, &semaforoP);
    }
    while (semforoC > 0)
    {
        sem_wait(sem_cons1);
        sem_getvalue(sem_cons1, &semforoC);
    }
    while (semaforoP2 > 0)
    {

        sem_wait(sem_prod2);
        sem_getvalue(sem_prod2, &semaforoP2);
    }
    while (semforoC2 > 0)
    {

        sem_wait(sem_cons2);
        sem_getvalue(sem_cons2, &semforoC2);
    }
    while (semforoCG > 0)
    {

        sem_wait(sem_consG);
        sem_getvalue(sem_consG, &semforoCG);
    }
    if (semaforoPG < 2)
    {
        while (semaforoPG < 2)
        {
            sem_post(sem_prodG);
            sem_getvalue(sem_prodG, &semaforoPG);
            printf("\n%d\n",semaforoPG);
        }
    }
    else if (semaforoPG > 2)
    {
        while (semaforoPG > 2)
        {
            sem_wait(sem_prodG);
            sem_getvalue(sem_prodG, &semaforoPG);
            printf("\n%d\n",semaforoPG);
        }
    }
    sem_post(sem_prod1);
    sem_post(sem_prod2);
    sem_getvalue(sem_prod1, &semaforoP);
    sem_getvalue(sem_prod2, &semaforoP2);
    printf("\n->Valor del semaforo Productor1[%d] Consumidor1[%d] Productor2[%d] Consumidor2[%d] ProductorG[%d] ConsumidorG[%d]\n", semaforoP, semforoC, semaforoP2, semforoC2, semaforoPG, semforoCG);

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

    pthread_t *prodth = NULL, *consth = NULL,*Ver=NULL;
    prodth = (pthread_t *)malloc(sizeof(pthread_t *) * nprod);
    consth = (pthread_t *)malloc(sizeof(pthread_t *) * ncons);
    Ver = (pthread_t *)malloc(sizeof(pthread_t *));

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
        Productors[i].Termine = 0;
        ExtraP -= (ExtraP > 0 ? 1 : 0);
        printf("\nPoductor[%d][%d,%d)\n", Productors[i].id, Productors[i].inicio, Productors[i].Final);
        pthread_create(&prodth[i], NULL, (void *)productor, (void *)&Productors[i]);
    }

    for (int i = 0; i < ncons; i++)
    {
        Consumidors[i].id = i + 1;        
        pthread_create(&consth[i], NULL, (void *)consumidor, (void *)&Consumidors[i]);
    }
     pthread_create(&Ver[0], NULL, (void *)Verificar, (void *)&Productors);

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
    sem_close(sem_prodG);
    sem_close(sem_consG);

    return 0;
}
//gcc Dos_Criticas.c  -lpthread -o Dos
// ./Dos nprod ncons nProducciones
