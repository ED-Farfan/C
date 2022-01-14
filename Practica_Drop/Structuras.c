#include <stdio.h>
#include <stdlib.h>
#include <math.h>


typedef struct
{
    char *Nombre;
    char *FechaM;
} Archivo;
typedef struct ListArchiv
{
    Archivo *arch;
    struct ListArchiv *Sig;
} ListArchiv;

typedef struct ListHashArch
{
    ListArchiv *raiz;
    int id;
    struct ListHashArch *Sig;
} ListHashArch;

Archivo *Crear_Archivo(char *Nombre, char *Fecha);
ListArchiv *NodoArchivo(char *Nombre, char *Fecha);
void recorrerListaArchivo(ListArchiv *raiz);
ListHashArch *NodoHashArchiv(char *Nombre, char *Fecha);

void AgregarDatos(char **almacenar, char *contenido);
int contar_Caracteres(char *Texto);
void limpiar(char *cad, int num);
void ListaArchivo(ListArchiv **r,char *Nombre, char *Fecha);
int HASH(char cad[]);
int tam_Cad(char cad[]);
int CadenasIguales(char a[],char b[]);

int main(int argc, char const *argv[])
{
    char *cad="Holaf",*cad2="Hola";
    ListArchiv *r = NULL;    
    ListaArchivo(&r,"David.txt", "12/23/22");
    ListaArchivo(&r,"David.txt", "12/33/342");
    ListaArchivo(&r,"Davidsdsdsds.txt", "12/33/34dsds");
    ListaArchivo(&r,"Dasdasdasdasdasdasds.txt", "4dsds");
    ListaArchivo(&r,"t", "4s");
    recorrerListaArchivo(r);
    printf("\n\nCOMPARO %s %s %i\n",cad,cad2,CadenasIguales(cad,cad2));
    return 0;
}
ListHashArch *NodoHashArchiv(char *Nombre, char *Fecha){
    ListHashArch *nuevo = NULL;
    nuevo = (ListHashArch*)malloc(sizeof(ListArchiv));
    nuevo ->id = HASH(Nombre);
    ListaArchivo(&(nuevo->raiz),Nombre, Fecha);
    nuevo->Sig=NULL;
}
void recorrerListaArchivo(ListArchiv *raiz){
    ListArchiv *r = raiz;
    while (r != NULL)
    {
        printf("\n\t->%s \n\t\t->Fecha: %s\n", r->arch->Nombre,r->arch->FechaM);
        r = r->Sig;
    }
    
}
void ListaArchivo(ListArchiv **r,char *Nombre, char *Fecha){
    if(*r == NULL){
        *r =NodoArchivo(Nombre,Fecha);
    }else
    {
        ListArchiv *i = *r;
        while (i->Sig != NULL)
        {
            i = i->Sig;
        }
        i->Sig = NodoArchivo(Nombre,Fecha);
    }    
}

ListArchiv *NodoArchivo(char *Nombre, char *Fecha){
    ListArchiv *nuevo = NULL;
    nuevo = (ListArchiv*)malloc(sizeof(ListArchiv));
    nuevo->Sig=NULL;
    nuevo->arch=Crear_Archivo(Nombre, Fecha);
    return nuevo;
}

Archivo *Crear_Archivo(char *Nombre, char *Fecha)
{    
    Archivo *r = NULL;    
    r = (Archivo *)malloc(sizeof(Archivo));
    r->Nombre = NULL;
    r->FechaM = NULL;
        
    AgregarDatos(&(r->Nombre), Nombre);
        
    AgregarDatos((&r->FechaM), Fecha);    
    
    return r;
}
int HASH(char cad[]){
    int i =0,aux=0;
    int res = 0;
    int pot=0;
    while (cad[i]!='\0')
    {
        aux = cad[i];
        pot = pow(aux,i);
        res+=(pot >0)?(pot%25000):0;
        i++;
    }
    return res;
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
        *(almacenar) = (char *)malloc(sizeof(char) * (datos+1));
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
int tam_Cad(char cad[]){
    int i = 0;
    if(cad != NULL){
        while (cad[i]!='\0')
        {
            i++;
        }        
    }
    return i;
}
int CadenasIguales(char a[],char b[]){
    if(a != NULL && b!= NULL){
        int i = 0,ban=1;
        while (a[i]!='\0' && b[i]!='\0')
        {
            if(a[i]!=b[i]){
                ban = 0;
                break;
            }
            i++;
        }
        if(ban == 1){
            if(a[i] == b[i]){
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