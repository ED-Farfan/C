#include <stdio.h>
#include <stdlib.h>


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