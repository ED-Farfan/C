// Ficheros de cabecera
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
//netbd.h es necesitada por la estructura hostent
int Tamanio_Char(char *c);
void Copiar_cadenas(char *a, char **r);
void Multiplicar_cadena(char **a, int m);
void limpiar(char *cad, int tam);
void EnteroACaracter(char **cad, int numero);
void Contatenar_Cadenas(char *a, char *b, char **r);
int Comparar_Char(char *a, char *b);
////
#define MAXIMO 1500
int main(int argc, char *argv[])
{

    if (argc > 2)
    {
        //Primer paso, definir variables
        char *ip;
        int fd, numbytes, puerto;
        puerto = atoi(argv[2]);
        ip = argv[1];
        struct hostent *he;
        /* estructura que recibirÃ¡ informaciÃ³n sobre el nodo remoto */
        struct sockaddr_in server;
        /* informaciÃ³n sobre la direcciÃ³n del servidor */

        if ((he = gethostbyname(ip)) == NULL)
        {
            /* llamada a gethostbyname() */
            printf("gethostbyname() error\n");
            exit(-1);
        }

        //Paso 2, definicion de socket
        if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        {
            /* llamada a socket() */
            printf("socket() error\n");
            exit(-1);
        }
        //Datos del servidor
        server.sin_family = AF_INET;
        server.sin_port = htons(puerto);
        server.sin_addr = *((struct in_addr *)he->h_addr);
        /*he->h_addr pasa la informaciÃ³n de ``*he'' a "h_addr" */
        bzero(&(server.sin_zero), 8);

        //Paso 3, conectarnos al servidor
        if (connect(fd, (struct sockaddr *)&server,
                    sizeof(struct sockaddr)) == -1)
        {
            /* llamada a connect() */
            printf("connect() error\n");
            exit(-1);
        }

        char buf[1510], res[MAXIMO + 10], salir[] = "Salir", servi[100];
        servi[0] = '\0';
        buf[0] = '\0';
        res[0] = '\0';
        FILE *F = NULL;
        char *mandar = NULL;
        printf("\nIngrese el nombre del archivo a leer\n->");
        fgets(res, 800, stdin);
        for (int i = 0; i < 800; i++)
        {
            if(res[i]=='\n'){
                res[i]='\0';
                break;
            }
        }
        printf("\n Busco -%s-", res);
        Contatenar_Cadenas("Copia/", res, &mandar);
        printf("\n Mando -%s-", mandar);        
        int tam = Tamanio_Char(mandar);
        F = fopen(res, "rb");
        if (F != NULL)
        {
            send(fd, mandar, tam, 0);

            int n;
            int count = 0;
            int aux = 0, ii = 0;
            while (!feof(F))
            {
                n = fread(buf, 1, MAXIMO, F);
                buf[n] = '\0';
                ii = 0;
                for (int i = 0; i < n; i++)                
                {
                    if (buf[i] != '\0')
                    {
                        if (ii + 1 < MAXIMO)
                        {
                            res[ii] = buf[i];
                            //printf("\n\tMandare %c ASCII: %d  iterador: %d  ", res[ii], res[ii], ii);
                            ii++;
                        }
                        else
                        {
                            res[ii] = '\0';
                            send(fd, res, ii, 0);
                            printf("\nEnvie %d bytes", ii);
                            count += ii;
                            if ((numbytes = recv(fd, servi, 100, 0)) == -1)
                            {
                                printf("Error en recv() \n");
                                exit(-1);
                            }
                            servi[numbytes] = '\0';
                            printf("\nServidor: %s \n", servi);
                            ii = 0;
                            i--;
                        }
                    }
                    else
                    {
                        if ((ii + 4) < MAXIMO)
                        {
                            res[ii] = 'Z';
                            //printf("\n\tMandare %c ASCII: %d  iterador: %d  ", res[ii], res[ii], ii);
                            ii++;
                            res[ii] = 'E';
                            //printf("\n\tMandare %c ASCII: %d  iterador: %d  ", res[ii], res[ii], ii);
                            ii++;
                            res[ii] = 'R';
                            //printf("\n\tMandare %c ASCII: %d  iterador: %d  ", res[ii], res[ii], ii);
                            ii++;
                        }
                        else
                        {
                            res[ii] = '\0';
                            send(fd, res, ii, 0);
                            printf("\nEnvie %d bytes", n);
                            count += ii;
                            if ((numbytes = recv(fd, servi, 100, 0)) == -1)
                            {
                                printf("Error en recv() \n");
                                exit(-1);
                            }
                            servi[numbytes] = '\0';
                            printf("\nServidor: %s \n", servi);
                            ii = 0;
                            i--;
                        }
                    }
                }
                if (ii > 0)
                {
                    res[ii] = '\0';
                    send(fd, res, ii, 0);
                    printf("\nEnvie %d bytes", n);
                    count += ii;
                    if ((numbytes = recv(fd, servi, 100, 0)) == -1)
                    {
                        printf("Error en recv() \n");
                        exit(-1);
                    }
                    servi[numbytes] = '\0';
                    printf("\nServidor: %s \n", servi);
                    ii = 0;
                }

                //tam = Tamanio_Char(buf);
            }
            /*char a ='\0';
        for (int i = 0; i < 1500; i++)
        {
            a = i;
            printf("\n Caracter: %c, ASCII: %d  I[%d]",a,a,i);
            buf[0] = a;
            send(fd, buf, 1, 0);
            //tam = Tamanio_Char(buf);
            if ((numbytes = recv(fd, res, 100, 0)) == -1)
            {
                
                printf("Error en recv() \n");
                exit(-1);
            }
            res[numbytes] = '\0';
            printf("\nServidor: %s \n", res);
        }*/
            printf("\nMande en total: %d bytes\n", count);
            send(fd, "Salir", 5, 0);
            
        }else
        {
            printf("\nNO EXISTE EL ARCHIVO: %s\n",res);
        }
        
        close(fd);
    }
    else
    {
        printf("No se ingreso el ip y puerto por parametro\n");
    }
}
int Tamanio_Char(char *c)
{
    if (c != NULL)
    {
        int i = 0;
        while (c[i] != '\0')
        {
            i++;
        }
        return i;
    }
    else
    {
        return 0;
    }
}
void Copiar_cadenas(char *a, char **r)
{

    if (a != NULL && *r == NULL)
    {
        char *res = NULL;
        int longitud = Tamanio_Char(a);
        res = (char *)malloc((longitud + 1) * sizeof(char));
        for (int i = 0; i < longitud; i++)
        {
            res[i] = a[i];
        }
        res[longitud] = '\0';
        *r = res;
    }
}
void Multiplicar_cadena(char **a, int m)
{

    if (*a != NULL && m >= 2)
    {
        char *u = *a;
        char *res = NULL;
        int longitud = Tamanio_Char(*a);
        res = (char *)malloc((longitud) * sizeof(char) * m + 1);
        int apun = 0;
        int max = ((longitud) * sizeof(char) * m + 1);
        for (int j = 0; j < m; j++)
        {
            for (int i = 0; u[i] != '\0'; i++)
            {
                res[apun] = u[i];
                apun++;
            }
        }
        res[apun] = '\0';

        *a = res;
    }
    else if (*a != NULL && m == -1)
    {
        char *u = *a;
        char *res = NULL;
        int longitud = Tamanio_Char(*a);
        res = (char *)malloc((longitud) * sizeof(char) + 1);

        int max = ((longitud) * sizeof(char) + 1);
        int apun = max - 2;
        for (int i = 0; i < longitud; i++)
        {
            res[apun] = u[i];

            apun--;
        }

        res[max - 1] = '\0';

        *a = res;
    }
}
void limpiar(char *cad, int tam)
{
    for (int i = 0; i < tam; i++)
    {
        cad[i] = '\0';
    }
}
void EnteroACaracter(char **cad, int numero)
{

    if (*cad == NULL)
    {
        int cont = 0;
        int div = numero, res = 0;
        while (div != 0)
        {
            res = div % 10;
            div = div / 10;
            cont++;
        }
        div = numero;
        char *cadn = NULL;
        cadn = (char *)malloc((cont) * sizeof(char) + 1);

        limpiar(cadn, cont + 1);

        for (int i = 0; i < cont; i++)
        {
            res = div % 10;
            div = div / 10;
            cadn[i] = res + '0';
        }
        cadn[cont] = '\0';
        Multiplicar_cadena(&cadn, -1);
        *cad = cadn;
    }
}
void Contatenar_Cadenas(char *a, char *b, char **r)
{

    if (a != NULL && b != NULL && *r == NULL)
    {
        int tam_a = Tamanio_Char(a);
        int tam_b = Tamanio_Char(b);
        char *res = (char *)malloc((tam_a + tam_b) * sizeof(char) + 1);
        int apuntador = 0;
        for (int i = 0; i < tam_a; i++)
        {
            res[i] = a[i];
            apuntador++;
        }
        for (int i = 0; i < tam_b; i++)
        {
            res[apuntador] = b[i];
            apuntador++;
        }
        res[tam_a + tam_b] = '\0';
        *r = res;
    }
}
int Comparar_Char(char *a, char *b)
{
    if (a != NULL && b != NULL)
    {
        int i = 0, j = 0;
        while ((a[i] != '\0' && b[i] != '\0') && (a[i] != '\n' && b[i] != '\n'))
        {
            j = (a[i] == b[i] ? 1 : 0);
            if (j == 0)
                break;
            i++;
        }
        return j;
    }
    else
    {
        return -1;
    }
}
//192.168.1.73