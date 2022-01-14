// Ficheros de cabecera
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include "Cadenas.h"
#define MAXIMO 1500

void *Atender(void *args);
void recibir_Archivo(int f2,char *Cliente);
//

//FunciÃ³n principal
int main(int argc, char **argv)
{

    if (argc > 1)
    {

        //Primer paso, definir variables
        int fd, *fd2 = NULL, longitud_cliente, puerto;
        puerto = atoi(argv[1]);

        //Se necesitan dos estructuras del tipo sockaddr
        //La primera guarda info del server
        //La segunda del cliente
        struct sockaddr_in server;
        struct sockaddr_in client;

        //Configuracion del servidor
        server.sin_family = AF_INET;         //Familia TCP/IP
        server.sin_port = htons(puerto);     //Puerto
        server.sin_addr.s_addr = INADDR_ANY; //Cualquier cliente puede conectarse
        bzero(&(server.sin_zero), 8);        //Funcion que rellena con 0's

        //Paso 2, definicion de socket
        if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            perror("Error de apertura de socket");
            exit(-1);
        }
        int activado = 1;
        setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &activado, sizeof(activado));
        //Paso 3, avisar al sistema que se creo un socket
        if (bind(fd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
        {
            printf("error en bind() \n");
            exit(-1);
        }

        //Paso 4, establecer el socket en modo escucha
        if (listen(fd, 5) == -1)
        {
            printf("error en listen()\n");
            exit(-1);
        }

        //Paso5, aceptar conexiones

        longitud_cliente = sizeof(struct sockaddr_in);
        /* A continuaciÃ³n la llamada a accept() */
        pthread_t *serv = NULL;
        while (1)
        {
            serv = NULL;
            fd2 = (int *)malloc(sizeof(int));
            serv = (pthread_t *)malloc(sizeof(pthread_t *));
            if ((*fd2 = accept(fd, (struct sockaddr *)&client, &longitud_cliente)) == -1)
            {
                printf("error en accept()\n");
                exit(-1);
            }
            pthread_create(serv, NULL, (void *)Atender, (void *)fd2);

            fd2 = NULL;
        }

        close(fd);
    }
    else
    {
        printf("NO se ingreso el puerto por parametro\n");
    }

    return 0;
}


void *Atender(void *args)
{
    int *f2 = (int *)args;
    int fd2 = *f2;
    char buf[MAXIMO+1];
    buf[0]='\0';
    int numbytes = 0;    
    if ((numbytes = recv(fd2, buf, MAXIMO, 0)) == -1)
    {
        /* llamada a recv() */
        printf("Error en recv() \n");
        exit(-1);
    }
    buf[numbytes]='\0';
    printf("\nRecibi %s ", buf);
    if(CadenasIguales(buf,"NEW-ARCH")==1){
        recibir_Archivo(fd2,"Cliente-1");
    }
    free(f2);
    close(fd2); /* cierra fd2 */
}
void recibir_Archivo(int fd2,char *Cliente){
    char buf[MAXIMO+10], *res= NULL, nombre[MAXIMO+10], Guardar[MAXIMO+10];
    buf[0]='\0';
    nombre[0] = '\0';
    Guardar[0] = '\0';
    int numbytes = 0;    
    AgregarDatos(&res,"ARCH-OK");
    int tam =  contar_Caracteres(res);
    send(fd2, res, tam, 0);
    
    FILE *F = NULL;

    if ((numbytes = recv(fd2, nombre, MAXIMO, 0)) == -1)
    {
        /* llamada a recv() */
        printf("Error en recv() \n");
        exit(-1);
    }
    printf("\nRecibi %s ", nombre);
    for (int i = 0; i < numbytes; i++)
    {
        nombre[i] = nombre[i] == '\n' ? '\0' : nombre[i];
    }
    AgregarDatos(&res,"ARCH-OK");
    send(fd2, "NOM-OK", 6, 0); // Contestamos con un ASK
    res[0]='\0';
    AgregarDatosEnCadena(&res,"DATO-OK");
    F = fopen(nombre, "wb");
    if (F != NULL && numbytes != -1)
    {
        int cont = 0;
        int aux = 0;
        int u = 0;
        int ii = 0;
        if (numbytes != -1)
        {
            while (u == 0){
                if ((numbytes = recv(fd2, buf, MAXIMO, 0)) == -1)
                {
                    /* llamada a recv() */
                    printf("Error en recv() \n");
                    exit(-1);
                }
                buf[numbytes]='\0';
                if (numbytes < 9 && CadenasIguales(buf,"END-ARCH"))
                {
                    printf("\nSALIR");
                    u = 2;                    
                }
                else
                {
                    printf("\nRecibi %d bytes -%s-", numbytes,buf);
                    cont += numbytes;
                    fwrite(buf, 1, numbytes, F);
                    //printf("\nRecibi: %s ASCII: %d",buf,buf[0]);                    
                    send(fd2, res, 7, 0); // Contestamos con un ASK
                }
            }
            fclose(F);
            printf("\nCerre Archivo\n");
            printf("\nRecibi en Total: %d bytes", cont);
        }
    }
}
//gcc semaforos.c  -lpthread -o semaforos_cruzados