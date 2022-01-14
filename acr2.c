#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

//paso 3: creacion de la funcion del hilo
void * function(void * parametro){
  int variable = *((int*)parametro);
  printf("hola Mundo desde el hilo: %d\n", variable);
  pthread_exit(NULL);
}

int main(int argc, char *argv[]){
    int value;
    pthread_t thread;
    //paso 0 : recepcion de numero de hilos a crear
    if(argc == 2){
        value = atoi(argv[1]);
    }else{
        printf("ingresar el numero de hilos\n");
    }
   

    //paso 1: creacion de los hilos 
    for(int i=0; i<value; i++){
        pthread_create(&thread, NULL, function, &i);
    }

    //paso 2: el padre espera a que los hijos terminen su ejecucion 
    pthread_join(thread, NULL);

    exit(0);
}

//gcc acr.c -o hilos -lpthread
