// Ficheros de cabecera
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
//netbd.h es necesitada por la estructura hostent
int Tamanio_Char(char *c);
void Copiar_cadenas(char *a, char **r);
void Multiplicar_cadena(char **a, int m);
void limpiar(char *cad, int tam);
void EnteroACaracter(char **cad, int numero);
void Contatenar_Cadenas(char *a, char *b, char **r);
int Comparar_Char(char *a, char *b);
////

////////
typedef struct Conversacion
{
    char *quien;
    char *Mensaje;
    struct Conversacion *p;
} Conversacion;
///
Conversacion *nodo(char *quie, char *text);
int Agregar(char *quie, char *text, Conversacion **top, Conversacion **r);
void leer(Conversacion *r);
int borrar_todo(Conversacion **r,Conversacion **t);
//
int main(int argc, char *argv[])
{

    if (argc > 2)
    {
        //Primer paso, definir variables
        char *ip;
        int fd, numbytes, puerto;
        char buf[100];
        puerto = atoi(argv[2]);
        ip = argv[1];
        Conversacion * raiz =NULL,*top=NULL;
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

        if ((numbytes = recv(fd, buf, 100, 0)) == -1)
        {
            /* llamada a recv() */
            printf("Error en recv() \n");
            exit(-1);
        }

        buf[numbytes] = '\0';

        printf("Mensaje del Servidor: %s\n", buf);
        /* muestra el mensaje de bienvenida del servidor =) */
        send(fd, "\nHola desde cliente\n", 20, 0);
        char env[800];
        while (1)
        {
            if ((numbytes = recv(fd, buf, 100, 0)) == -1)
            {
                /* llamada a recv() */
                printf("Error en recv() \n");
                exit(-1);
            }
            buf[numbytes] = '\0';
            Agregar("Servidor", buf, &top, &raiz);
            leer(raiz);
            
            if (Comparar_Char(buf, "Salir") == 1)
            {
                break;
            }

            printf("\nIngrese mensaje a enviar: \n-> ");
            fgets(env, 800, stdin);
            int tam = Tamanio_Char(env);
            printf("\nenviar: %s [%d] \n ", env, tam);
            send(fd, env, tam, 0);
            Agregar("yo",env, &top, &raiz);
            leer(raiz);
            if (Comparar_Char(env, "Salir") == 1)
            {
                break;
            }
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

Conversacion *nodo(char *quie, char *text)
{
    if (quie != NULL && text != NULL)
    {
        Conversacion *a = NULL;
        a = malloc(sizeof(Conversacion));
        a->Mensaje=NULL;
        a->quien=NULL;
        Copiar_cadenas(quie, &a->quien);
        Copiar_cadenas(text, &a->Mensaje);
        a->p = NULL;
        return a;
    }
    else
    {
        return NULL;
    }
}
int Agregar(char *quie, char *text, Conversacion **top, Conversacion **r)
{
    if (quie != NULL && text != NULL)
    {
        if ((*top) != NULL)
        {
            (*top)->p = nodo(quie, text);
            (*top) = (*top)->p;
            return 1;
        }
        else
        {
            (*r) = nodo(quie, text);
            (*top) = (*r);
            return 1;
        }
    }else
    {
        return -1;
    }
    
}
void leer(Conversacion *r){
    if(r != NULL){
        system("clear");
        Conversacion *i = r;
        printf("\n\t\tCONVERSACION\n\n");
        while(i!=NULL){
            printf("\n->[%s]: %s",i->quien,i->Mensaje);
            i = i->p;
        }
        printf("\n\n");
    }
}
int borrar_todo(Conversacion **r,Conversacion **t){
    (*t)=NULL;
    while ((*r)!=NULL)
    {
        Conversacion *aux = (*r);
        (*r)=(*r)->p;
        free(aux);
    }
    return 1;    
}