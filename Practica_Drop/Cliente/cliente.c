#include "Cadenas.h"
#include <time.h>
#include <sys/types.h>
#include <string.h>
#include <dirent.h>

#include <sys/stat.h>
#include <stdlib.h>
/////
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>



int Existe_Carpeta(char nombre[]);
int crear_Carpeta(char *Carpeta);

struct stat *Fechas_Archivos(char *cad);
typedef struct
{
    char *Nombre;
    struct tm *FechaM;
} Archivo;

typedef struct ListArchiv
{
    Archivo *arch;
    struct ListArchiv *Sig;
} ListArchiv;
///////////////////////
ListArchiv *Archivos_Carpeta(char carpeta[]);
Archivo *Crear_Archivo_Sin_Fecha(char *Nombre);
void ListaArchivo_Sin_Fecha(ListArchiv **r, char *Nombre);
ListArchiv *NodoArchivo_Sin_Fecha(char *Nombre);
void recorrerListaArchivo(ListArchiv *raiz);
void Asignar_Fechas_Modificacion(ListArchiv *raiz, char *Direc);
void crear(char *Direc);
void Almacenar_Archivo(char *Dir, Archivo *c);
void Guardar_Archivos(ListArchiv *r, char *Dir);
void listado();
///////////////////
int main(int argc, char const *argv[])
{
    ListArchiv *r = NULL;
    r = Archivos_Carpeta("Documentos");
    recorrerListaArchivo(r);
    Asignar_Fechas_Modificacion(r, NULL);
    printf("\n\n\n\n");
    recorrerListaArchivo(r);
    Guardar_Archivos(r, NULL);
    Guardar_Archivos(r, "Esta/Prueba/Es/Para.txt");    
    char *Cada="Cadena Nueva";
    AgregarDatosEnCadena(&Cada," Que tal");
    CambiarCaracter(Cada,' ','_');
    printf("\n\t\tPruebas:%s\n",Cada);
    free(r);
    printf("\nDireccion: -%s-\n", Formatear_Cadena_Direccion("Documentos", NULL));
    listado();    
    return 0;
}
ListArchiv *Archivos_Carpeta(char carpeta[])
{
    ListArchiv *r = NULL;
    /* Con un puntero a DIR abriremos el directorio */
    DIR *dir;
    /* en *ent habrá información sobre el archivo que se está "sacando" a cada momento */
    struct dirent *ent;
    /* Empezaremos a leer en el directorio actual */
    dir = opendir(carpeta);
    /* Miramos que no haya error */
    if (dir == NULL)
    {
        printf("\nNo puedo abrir el directorio\n");
        return NULL;
    }

    /* Una vez nos aseguramos de que no hay error, ¡vamos a jugar! */
    /* Leyendo uno a uno todos los archivos que hay */
    while ((ent = readdir(dir)) != NULL)
    {

        /* Nos devolverá el directorio actual (.) y el anterior (..), como hace ls */
        if ((strcmp(ent->d_name, ".") != 0) && (strcmp(ent->d_name, "..") != 0))
        {
            /* Una vez tenemos el archivo, lo pasamos a una función para procesarlo. */
            ListaArchivo_Sin_Fecha(&r, ent->d_name);
        }
    }
    closedir(dir);
    return r;
}
Archivo *Crear_Archivo_Sin_Fecha(char *Nombre)
{
    Archivo *r = NULL;
    r = (Archivo *)malloc(sizeof(Archivo));
    r->Nombre = NULL;
    r->FechaM = NULL;
    r->FechaM = (struct tm *)malloc(sizeof(struct tm));
    AgregarDatos(&(r->Nombre), Nombre);
    return r;
}
void ListaArchivo_Sin_Fecha(ListArchiv **r, char *Nombre)
{
    if (*r == NULL)
    {
        *r = NodoArchivo_Sin_Fecha(Nombre);
    }
    else
    {
        ListArchiv *i = *r;
        while (i->Sig != NULL)
        {
            i = i->Sig;
        }
        i->Sig = NodoArchivo_Sin_Fecha(Nombre);
    }
}

ListArchiv *NodoArchivo_Sin_Fecha(char *Nombre)
{
    ListArchiv *nuevo = NULL;
    nuevo = (ListArchiv *)malloc(sizeof(ListArchiv));
    nuevo->Sig = NULL;
    nuevo->arch = Crear_Archivo_Sin_Fecha(Nombre);
    return nuevo;
}

void recorrerListaArchivo(ListArchiv *raiz)
{
    if (raiz != NULL)
    {
        printf("\n\t\t\tARCHIVOS\n");
        ListArchiv *r = raiz;
        while (r != NULL)
        {
            char date[50];
            time_t t = mktime(r->arch->FechaM);
            printf("\n\t->%s \n\t\t->Fecha De Modificacion: %s \n", r->arch->Nombre, formatdate(date, t));

            r = r->Sig;
        }
        printf("\n\n");
    }
    else
    {
        printf("\nVacio\n");
    }
}

///////////////////////////////////////////////////////////////////////////////

struct stat *Fechas_Archivos(char *cad)
{
    struct stat *file_info = malloc(sizeof(struct stat));
    if (lstat(cad, file_info) != 0)
    {
        printf("\nNo encontre %s\n", cad);
        return NULL;
    }
    else
    {
        return file_info;
    }
}

void Asignar_Fechas_Modificacion(ListArchiv *raiz, char *Direc)
{
    if (raiz != NULL)
    {
        char *Carpeta = NULL;
        ListArchiv *r = raiz;
        if (Direc == NULL)
        {
            AgregarDatos(&Carpeta, "Documentos/");
        }
        else
        {
            AgregarDatos(&Carpeta, Direc);
        }

        while (r != NULL)
        {
            char *Ruta = NULL;
            Ruta = UnirDosCadenas(Carpeta, r->arch->Nombre);
            struct stat *fecha = Fechas_Archivos(Ruta);
            if (fecha == NULL)
            {
                printf("Valiomadre");
            }
            else
            {
                char date[50];
                struct tm *timeinfo = localtime(&(fecha->st_mtime));                
                *(r->arch->FechaM) = *timeinfo;
                r = r->Sig;
            }
            free(Ruta);
            free(fecha);
        }
    }
    else
    {
        printf("\nVacio\n");
    }
}

////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////
void crear(char *Direc)
{
    if (Direc != NULL)
    {
        FILE *arch;
        arch = fopen(Direc, "wb");
        if (arch == NULL)
            exit(1);
        fclose(arch);     
        
    }
}
void Almacenar_Archivo(char *Dir, Archivo *c)
{
    if (Dir != NULL && c != NULL)
    {
        FILE *arch;
        arch = fopen(Dir, "ab");
        if (arch == NULL)
            exit(1);
        Archivo *A = c;
        fwrite(A, sizeof(Archivo), 1, arch);
        fclose(arch);
        printf("\nSe almaceno: %s\n", c->Nombre);
    }
}
void Guardar_Archivos(ListArchiv *r, char *Dir)
{
    if (Dir == NULL)
    {
        if (Existe_Carpeta("Base_Datos") == 0)
        {
            crear_Carpeta("Base_Datos");
        }
        crear("Base_Datos/Archi.dat");
        char dir[] = "Base_Datos/Archi.dat";
        ListArchiv *u = r;
        int cont = 0;
        while (u != NULL)
        {
            Almacenar_Archivo(dir, u->arch);
            cont++;
            u = u->Sig;
        }
        printf("\nTermine Almacene: %d Archivos\n", cont);
    }
    else
    {
        int diag = Contar_Caracter(Dir, '/');
        char **Ruta = Descomponer_Cadena(Dir, '/');
        char *rut = '\0'; 
        for (int i = 0; i < diag; i++)        
        {            
            AgregarDatosEnCadena(&rut,Ruta[i]);
            CambiarCaracter(rut,' ','_');
            if (Existe_Carpeta(rut) == 0)
            {
                crear_Carpeta(rut);    
            }
            AgregarDatosEnCadena(&rut,"/");            
        }
        AgregarDatosEnCadena(&rut,Ruta[diag]);       
        crear(rut);        
        ListArchiv *u = r;
        int cont = 0;
        while (u != NULL)
        {
            Almacenar_Archivo(Dir, u->arch);
            cont++;
            u = u->Sig;
        }        
        free(rut);        
        printf("\nTermine Almacene: %d Archivos\n", cont);
    }
}
void listado()
{
    FILE *arch;
    arch = fopen("Base_Datos/Archi.dat", "rb");
    if (arch == NULL)
        exit(1);
    Archivo ar;
    fread(&ar, sizeof(Archivo), 1, arch);
    while (!feof(arch))
    {
        char date[50];
        time_t t = mktime(ar.FechaM);
        printf("Archivo:%s \n\tFecha_Modificacion:%s\n", ar.Nombre, formatdate(date, t));

        fread(&ar, sizeof(Archivo), 1, arch);
    }
    fclose(arch);
}

////////////////////////MANEJO DE CARPETAS
int Existe_Carpeta(char nombre[])
{
    DIR *dirp = NULL;
    struct dirent *direntp;
    dirp = opendir(nombre);
    if (dirp == NULL)
    {
        return 0;
    }
    else
    {
        closedir(dirp);
        return 1;
    }
}
int crear_Carpeta(char *Carpeta)
{
    if (Carpeta != NULL)
    {
        int i = mkdir(Carpeta, 0700);
        i = (i == 0) ? 1 : 0;
        return i;
    }
    else
    {
        return -1;
    }
}