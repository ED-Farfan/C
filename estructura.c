#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

struct nota
{
    int x;
    char t[1000];    
};
char* enteroACaracter(int numero){
    char *c=NULL;
    c=(char*)malloc(sizeof(char)*1000);
    sprintf(c, "%i", numero);
    return c;
}
void Copiar(char * ori,char * cop){    
    int i=0;
    while(ori[i]!='\0'){
        cop[i] = ori[i];
        i++;
    }    
    ori[i]='\0';    
    cop[i]='\0';    
    
}
/* devuelve la cantidad de caracteres en cadena sin contar el '\0' */
int largo_cadena(char cadena[])
{
	int largo=0;
    
	while(cadena[largo]!='\0'){
        largo++;
    } 
	return largo;
}

void Concatenar(char * ori,char * cop){
    int i = 0;
    int ii = largo_cadena(cop);
    
    //ii = (ii==0?:0,ii++);
    
    for ( i = 0; ori[i] != '\0'; i++)
    {
       cop[ii+i] = ori[i];
    
    }
    cop[ii+i]='-';    
    cop[ii+i+1]='\0';    
    
}
//paso 3: creacion de la funcion del hilo

void *function(void *parametro)
{
    
    struct nota *com=((struct nota *)parametro);
    printf("hola Mundo desde el hilo: %d %s\n", com->x,com->t);    
    //pthread_exit(NULL);
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    int value;
    pthread_t *thread = NULL;
    int *cas = NULL;
    struct nota *raiz=NULL;
    char aumen[1000];
    aumen[0]='\0';
    
    
    //paso 0 : recepcion de numero de hilos a crear
    if (argc == 2)
    {
        value = atoi(argv[1]);
        thread = (pthread_t *)malloc (value*sizeof(pthread_t));
        cas = (int*)malloc(value*sizeof(int));
        raiz = malloc(value*sizeof(struct nota));

    }
    else
    {
        printf("ingresar el numero de hilos\n");
        exit(0);
    }

    //paso 1: creacion de los hilos
    for (int i = 0; i < value; i++)
    {
        cas[i]=i;
        char *dato = enteroACaracter(i);
         fflush( stdin );
        raiz[i].t[0]='\0';
        Concatenar(dato,aumen);        
         fflush( stdin );
        Copiar(aumen,raiz[i].t);
         fflush( stdin );
        raiz[i].x = i;
        pthread_create(&thread[i], NULL, function,&raiz[i]);
    }

    //paso 2: el padre espera a que los hijos terminen su ejecucion

    for (int i = 0; i < value; i++)
    {
        pthread_join(thread[i], NULL);
    }
    free(thread);
    free(cas);
    free(raiz);
    exit(0);
}

//gcc acr.c -o hilos -lpthread
