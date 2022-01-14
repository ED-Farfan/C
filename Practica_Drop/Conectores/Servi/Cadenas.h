#include <time.h>
#include <stdlib.h>
#include <string.h>

void AgregarDatos(char **almacenar, char *contenido);
int contar_Caracteres(char *Texto);
void limpiar(char *cad, int num);
int CadenasIguales(char a[], char b[]);
char *formatdate(char *str, time_t val);
char *UnirDosCadenas(char *a, char *b);
char *Formatear_Cadena_Direccion(char cad[], char cadB[]);
int Contar_Espacios(char cad[]);
int Contar_Caracter(char cad[], char caracter);
int Copiar_Cadena_Hasta(char *cadena, char *Destino, char caracter, int tam_Destino, int inicio);
int Contar_Caracteres_Hasta(char cad[], char caracter, int inicio);
char **Descomponer_Cadena(char *Cadena, char caracter);
void AgregarDatosEnCadena(char **cadena,char *contenido);
void CambiarCaracter(char *Cadena,char caracter, char nuevo);


void CambiarCaracter(char *Cadena,char caracter, char nuevo){
    if(Cadena != NULL && caracter != '\0' && nuevo != '\0'){
        int i=0;
        while (Cadena[i]!='\0')
        {
            if(Cadena[i]==caracter){
                Cadena[i] = nuevo;
            }
            i++;
        }
        
    }
}
void AgregarDatosEnCadena(char **cadena,char *contenido){
    if(contenido != NULL && *(cadena)!=NULL){
        int datos = contar_Caracteres(*cadena);        
        datos += contar_Caracteres(contenido);
        char *cad = (char *)malloc(sizeof(char) * (datos + 2));
        int i=0,j=0;
        while ((*cadena)[i]!='\0')
        {
            cad[i]=(*cadena)[i];                 
            i++;
        }        
        while (contenido[j]!='\0')
        {
            cad[i]=contenido[j];
            i++;
            j++;
        }
        cad[i]='\0';        
         *(cadena) = cad;     
    }
    else if (contenido != NULL && *(cadena)==NULL)
    {
        int datos = contar_Caracteres(contenido);
        char *cad = (char *)malloc(sizeof(char) * (datos + 2));
        int i=0,j=0;        
        while (contenido[j]!='\0')
        {
            cad[i]=contenido[j];
            i++;
            j++;
        }
        cad[i]='\0';        
         *(cadena) = cad;   
    }
    
}
void AgregarDatos(char **almacenar, char *contenido)
{

    if (contenido != NULL)
    {
        if (*almacenar != NULL)
        {
            free(*almacenar);
            *almacenar = NULL;
        }
        int datos = contar_Caracteres(contenido);
        *(almacenar) = (char *)malloc(sizeof(char) * (datos + 1));
        limpiar(*almacenar, datos);
        int i = 0;
        while (contenido[i] != '\0')
        {

            (*almacenar)[i] = contenido[i];

            i++;
        }
        (*almacenar)[i] = '\0';
    }
}
int contar_Caracteres(char *Texto)
{
    int i = 0;

    while (Texto[i] != '\0')
    {
        i++;
    }
    return i;
}
void limpiar(char *cad, int num)
{

    for (int i = 0; i < num; i++)
    {
        cad[i] = '\0';
    }
}
int tam_Cad(char cad[])
{
    int i = 0;
    if (cad != NULL)
    {
        while (cad[i] != '\0')
        {
            i++;
        }
    }
    return i;
}
int CadenasIguales(char a[], char b[])
{
    if (a != NULL && b != NULL)
    {
        int i = 0, ban = 1;
        while (a[i] != '\0' && b[i] != '\0')
        {
            if (a[i] != b[i])
            {
                ban = 0;
                break;
            }
            i++;
        }
        if (ban == 1)
        {
            if (a[i] == b[i])
            {
                ban = 1;
            }
            else
            {
                ban = 0;
            }
        }
        return ban;
    }
    else
    {
        return -1;
    }
}
char *formatdate(char *str, time_t val)
{
    strftime(str, 36, "%d.%m.%Y %H:%M:%S", localtime(&val));
    return str;
}

char *UnirDosCadenas(char *a, char *b)
{
    if (a != NULL && b != NULL)
    {
        int datos = 0;
        char *res = NULL;
        datos = tam_Cad(a);
        datos += tam_Cad(b);
        res = (char *)malloc(sizeof(char) * (datos + 2));
        limpiar(res, datos);
        int i = 0, j = 0;
        while (a[i] != '\0')
        {
            res[i] = a[i];
            i++;
        }
        while (b[j] != '\0')
        {
            res[i] = b[j];
            i++;
            j++;
        }
        res[i] = '\0';
        return res;
    }
    else
    {
        return NULL;
    }
}
int Contar_Espacios(char cad[])
{
    if (cad != NULL)
    {
        int cont = 0, i = 0;
        while (cad[i] != '\0')
        {
            cont += (cad[i] == ' ') ? 1 : 0;
            i++;
        }
        return cont;
    }
    else
    {
        return -1;
    }
}
char *Formatear_Cadena_Direccion(char cad[], char cadB[])
{
    char *cadena = NULL;
    char *final = NULL;
    if ((cadB == NULL && cad == NULL) || cad == NULL)
    {
        return NULL;
    }
    else if (cadB != NULL && cad != NULL)
    {
        cadena = UnirDosCadenas(cad, cadB);
        int tam = 0, espa = 0;
        tam = tam_Cad(cadena);
        espa = Contar_Espacios(cadena);
        final = (char *)malloc(sizeof(char) * ((tam + espa) + 2));
    }
    else if (cadB == NULL && cad != NULL)
    {

        AgregarDatos(&cadena, cad);
        int tam = 0, espa = 0;
        tam = tam_Cad(cad);
        espa = Contar_Espacios(cad);
        final = (char *)malloc(sizeof(char) * ((tam + espa) + 2));
    }
    int i = 0, j = 0;
    while (cadena[i] != '\0')
    {
        if (cadena[i] == ' ')
        {
            final[j] = '\\';
            j++;
            final[j] = ' ';
            j++;
        }
        else
        {
            final[j] = cadena[i];
            j++;
        }
        i++;
    }
    free(cadena);
    final[j] = '\0';
    return final;
}
int Copiar_Cadena_Hasta(char *cadena, char *Destino, char caracter, int tam_Destino, int inicio)
{
    if (cadena != NULL && Destino != NULL)
    {
        int i = inicio;
        int j = 0;
        while (j < tam_Destino && cadena[i] != caracter && cadena[i] != '\0')
        {
            Destino[j] = cadena[i];
            j++;
            i++;
        }
        if (j >= tam_Destino)
        {
            j--;
        }
        Destino[j] = '\0';
        return i;
    }
}
int Contar_Caracteres_Hasta(char cad[], char caracter, int inicio)
{
    if (cad != NULL)
    {
        int i = inicio;
        while (cad[i] != '\0' && cad[i] != caracter)
        {
            i++;
        }
        return i - inicio;
    }
    else
    {
        return -1;
    }
}
char **Descomponer_Cadena(char *Cad, char caracter)
{
    if (Cad != NULL && caracter != '\0')
    {

        int num = Contar_Caracter(Cad, caracter) + 1;
        int i = 0;
        char **nuevo = NULL;
        nuevo = (char **)malloc(num * sizeof(char *));
        int datos = 0, inicio = 0, leidos = 0;
        char Prueba[50];
        for (int i = 0; i < num; i++)
        {
            leidos = Contar_Caracteres_Hasta(Cad, caracter, inicio) + 1;
            Copiar_Cadena_Hasta(Cad, Prueba, '/', 50, inicio);
            inicio += leidos;
            nuevo[i] = NULL;
            AgregarDatos(&nuevo[i], Prueba);
        }
        //free(nuevo);
        return nuevo;
    }
    else
    {
        return NULL;
    }
}
int Contar_Caracter(char *nu, char caracter)
{

    if (nu != NULL)
    {

        ;
        char *cad = NULL;
        AgregarDatos(&cad, nu);
        int conti = 0, i = 0;

        while (cad[i] != '\0')
        {

            conti += (cad[i] == caracter) ? 1 : 0;

            i++;
        }
        return conti;
    }
    else
    {
        return -1;
    }
}
