// Ficheros de cabecera
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
int Tamanio_Char(char *c);
void Copiar_cadenas(char *a, char **r);
void Multiplicar_cadena(char **a, int m);
void limpiar(char *cad, int tam);
void EnteroACaracter(char **cad, int numero);
void Contatenar_Cadenas(char *a, char *b, char **r);
int Comparar_Char(char *a,char *b);

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

//FunciÃ³n principal
int main(int argc, char **argv)
{

	if (argc > 1)
	{

		//Primer paso, definir variables
		int fd, fd2, longitud_cliente, puerto;
		puerto = atoi(argv[1]);
		Conversacion * raiz =NULL,*top=NULL;
		//Se necesitan dos estructuras del tipo sockaddr
		//La primera guarda info del server
		//La segunda del cliente
		struct sockaddr_in server;
		struct sockaddr_in client;

		//Configuracion del servidor
		server.sin_family = AF_INET;		 //Familia TCP/IP
		server.sin_port = htons(puerto);	 //Puerto
		server.sin_addr.s_addr = INADDR_ANY; //Cualquier cliente puede conectarse
		bzero(&(server.sin_zero), 8);		 //Funcion que rellena con 0's
		
		
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
		if ((fd2 = accept(fd, (struct sockaddr *)&client, &longitud_cliente)) == -1)
		{
			printf("error en accept()\n");
			exit(-1);
		}

		send(fd2, "Bienvenido a mi servidor.\n", 26, 0);
		char buf[100], env[800], res[800];
		int numbytes = recv(fd2, buf, 100, 0);
		buf[numbytes] = '\0';
		printf("Mensaje del Cliente: %s\n", buf);
		while (1)
		{
			printf("\nIngrese mensaje a enviar: \n-> ");
			fgets(env, 800, stdin);
			int tam = Tamanio_Char(env);
			printf("\nenviar: %s [%d] \n ", env, tam);
			send(fd2, env, tam, 0);
			if(Comparar_Char(env,"Salir")==1){
				break;
			}
			Agregar("Yo", env, &top, &raiz);
            leer(raiz);

			if ((numbytes = recv(fd2, buf, 100, 0)) == -1)
            {
                /* llamada a recv() */
                printf("Error en recv() \n");
                exit(-1);
            }
            buf[numbytes] = '\0';
            printf("Mensaje del Cliente: %s\n", buf);
			Agregar("Cliente", env, &top, &raiz);
            leer(raiz);
            if(Comparar_Char(buf,"Salir")==1){
                break;
            }
		}
		
		

		close(fd2); /* cierra fd2 */

		close(fd);
	}
	else
	{
		printf("NO se ingreso el puerto por parametro\n");
	}

	return 0;
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
int Comparar_Char(char *a,char *b){
	if(a != NULL && b != NULL){
		int i=0,j=0;
		while ((a[i] != '\0' && b[i] != '\0')&&(a[i] != '\n' && b[i] != '\n'))
		{
			j = (a[i]==b[i]? 1 : 0);
			if(j==0)break;			
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