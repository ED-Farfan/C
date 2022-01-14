#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include "Cadenas.h"

#define MAXIMO 1500

int Mandar_Archivos(char *ip, int puerto, char *Archivo, char*Fecha);
int Mandar_Datos(FILE *F,int fd, char *Archivo);
int Mandar_Archivos(char *ip, int puerto, char *Archivo,char *Fecha)
{
    int fd = 0, numbytes = 0;
    struct hostent *he;
    /* estructura que recibirÃ¡ informaciÃ³n sobre el nodo remoto */
    struct sockaddr_in server;
    /* informaciÃ³n sobre la direcciÃ³n del servidor */

    if ((he = gethostbyname(ip)) == NULL)
    {
        /* llamada a gethostbyname() */
        printf("gethostbyname() error\n");
        return -23;
    }
    //Paso 2, definicion de socket
    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        /* llamada a socket() */
        printf("socket() error\n");
        return -45;
    }
    //Datos del servidor
    server.sin_family = AF_INET;
    server.sin_port = htons(puerto);
    server.sin_addr = *((struct in_addr *)he->h_addr);
    /*he->h_addr pasa la informaciÃ³n de ``*he'' a "h_addr" */
    bzero(&(server.sin_zero), 8);

    if (connect(fd, (struct sockaddr *)&server,
                sizeof(struct sockaddr)) == -1)
    {
        /* llamada a connect() */
        printf("connect() error\n");
        return -2;
    }    
    FILE *F = NULL;    
    F = fopen(Archivo, "rb");
    if (F != NULL)
    {
        Mandar_Datos(F,fd,Archivo);

        fclose(F);
        
        close(fd);
        return 1;
    }
    else
    {
        close(fd);
        return -1;
    }
}
int Mandar_Datos(FILE *F,int fd, char *Archivo){
    int numbytes=0;
    char buf[MAXIMO + 10], res[MAXIMO + 10], Inicio[] = "NEW-ARCH", Fin[] = "END-ARCH", servi[MAXIMO + 10];
    servi[0] = '\0';
    buf[0] = '\0';
    res[0] = '\0';
    char **Lista = Descomponer_Cadena(Archivo, '/');
    int diag = Contar_Caracter(Archivo, '/');
    char *mandar = NULL;

    AgregarDatos(&mandar, Lista[diag]);
    int tam = contar_Caracteres(Inicio);
    printf("\n Mando -%s- Tamanio: %d", Inicio, tam);
    printf("\n Pase zona critica\n");
    send(fd, Inicio, tam, 0);
        int nui;
        nui = recv(fd, buf, MAXIMO, 0);
        buf[nui] = '\0';
        printf("\nServidor: -%s-\n",buf);

        tam = contar_Caracteres(mandar);
        send(fd, mandar, tam, 0);

        numbytes = recv(fd, servi, MAXIMO, 0);
        printf("\nServidor:%s\n", servi);

        int n;
        int count = 0;
        while (!feof(F))
        {
            n = fread(buf, 1, MAXIMO, F);
            buf[n] = '\0';
            send(fd, buf, n, 0);
            printf("\nMande  %d bytes\n", n);
            count += n;
            if ((numbytes = recv(fd, servi, MAXIMO, 0)) == -1)
            {
                printf("Error en recv() \n");
                return -11;
            }
            else
            {
                printf("\nServidor:%s\n", servi);
            }
        }       
        printf("\nMande en total: %d bytes\n", count);
        send(fd, "END-ARCH", 8, 0);        
}