#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[])
{

    /* Variables */
    DIR *dirp;
    struct dirent *direntp;

    /* Comprobamos los argumentos */
    if (argc != 2)
    {
        printf("Uso: %s directorio\n", argv[0]);
        exit(1);
    }

    /* Abrimos el directorio */
    dirp = opendir(argv[1]);
    if (dirp == NULL)
    {
        printf("Error: No se puede abrir el directorio\n");
        printf("\nHola:%d\n",mkdir(argv[1], 0700));
        exit(2);
    }

    /* Leemos las entradas del directorio */
    printf("i-nodo\toffset\t\tlong\tnombre\n");
    while ((direntp = readdir(dirp)) != NULL)
    {
        printf("%ld\t%ld\t%d\t%s\n", direntp->d_ino, direntp->d_off, direntp->d_reclen, direntp->d_name);
    }
    /* Cerramos el directorio*/
    closedir(dirp);
    return 0;
}