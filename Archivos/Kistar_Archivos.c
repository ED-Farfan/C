#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

/* Función para devolver un error en caso de que ocurra */
void error(const char *s);
/* Función que hace algo con un archivo */
void procesoArchivo(char *archivo);

int main()
{
    /* Con un puntero a DIR abriremos el directorio */
    DIR *dir;
    /* en *ent habrá información sobre el archivo que se está "sacando" a cada momento */
    struct dirent *ent;

    /* Empezaremos a leer en el directorio actual */
    dir = opendir(".");

    /* Miramos que no haya error */
    if (dir == NULL)
        error("No puedo abrir el directorio");

    /* Una vez nos aseguramos de que no hay error, ¡vamos a jugar! */
    /* Leyendo uno a uno todos los archivos que hay */
    while ((ent = readdir(dir)) != NULL)
    {
        /* Nos devolverá el directorio actual (.) y el anterior (..), como hace ls */
        if ((strcmp(ent->d_name, ".") != 0) && (strcmp(ent->d_name, "..") != 0))
        {
            /* Una vez tenemos el archivo, lo pasamos a una función para procesarlo. */
            procesoArchivo(ent->d_name);
        }
    }
    closedir(dir);

    return EXIT_SUCCESS;
}

void error(const char *s)
{
    /* perror() devuelve la cadena S y el error (en cadena de caracteres) que tenga errno */
    perror(s);
    exit(EXIT_FAILURE);
}

void procesoArchivo(char *archivo)
{
    /* Para "procesar", o al menos, hacer algo con el archivo, vamos a decir su tamaño en bytes */
    /* para ello haremos lo que vemos aquí: https://poesiabinaria.net/2010/04/tamano-de-un-fichero-en-c/ */
    FILE *fich;
    long ftam;

    fich = fopen(archivo, "r");
    printf("\nFichero: %d \n",fich);
    if (fich!=NULL)
    {
        fseek(fich, 0L, SEEK_END);
        ftam = ftell(fich);
        fclose(fich);
        /* Si todo va bien, decimos el tamaño */
        printf("%20s (%ld bytes)\n", archivo, ftam);
    }
    else
        /* Si ha pasado algo, sólo decimos el nombre */
        printf("%20s (No info.)\n", archivo);
    

    
}