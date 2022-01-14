#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

struct nota
{
    int max;
    int ant;
    int ant1;
    int hilo;
    char t[10000];    
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
    int nex=1;
    while(nex < com->max){
        printf("\n Hilo: %i , Maximo:%i, fibo: %i\n",com->hilo,com->max,nex);
        if(com->ant1==0 && com->ant==0){
            nex=1;
            com->ant1=1;
        }
        else if(com->ant1==1 && com->ant == 0){
            com->ant=1;
        }
        else
        {
            nex= com->ant1;
            com->ant1=com->ant;
            com->ant += nex;
            nex=com->ant;
            
        }                
    }
    
    //pthread_exit(NULL);
    pthread_exit(NULL);
}
int numero_de_Hilos(char*cad){
    int i = 0;
    int cont=0;
    while (cad[i] != '\0')
    {
        i++;        
        if(cad[i]==','){
            printf("\n%c\n",cad[i]);
            cont++;
        }
    }    
    cont++;
    return cont;    

}

int main(int argc, char *argv[])
{
    int value;
    pthread_t *thread = NULL;
    int *maxs = NULL;
    struct nota *raiz=NULL;
    char aumen[10000];
    aumen[0]='\0';
    
    
    //paso 0 : recepcion de numero de hilos a crear
    if (argc >= 0)
    {
        thread = (pthread_t *)malloc ((argc)*sizeof(pthread_t));
        maxs = (int*)malloc((argc)*sizeof(int));
        raiz = malloc((argc)*sizeof(struct nota));
        
        for(int i=1;i<(argc);i++){
            maxs[i]=atoi(argv[i]);            
            raiz[i].max=maxs[i];        
            raiz[i].ant=0;
            raiz[i].ant1=0;
            raiz[i].hilo=i;
            pthread_create(&thread[i], NULL, function,&raiz[i]);
        }
        


        


    }
    else
    {
        printf("ingresar el numero de hilos\n");
        exit(0);
    }

    

    //paso 2: el padre espera a que los hijos terminen su ejecucion
    for (int i = 1; i < argc; i++)
    {
        pthread_join(thread[i], NULL);
    }
    
    free(thread);
    free(maxs);
    free(raiz);
    exit(0);
}