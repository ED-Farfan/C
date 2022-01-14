#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    struct stat sb;

    if (argc != 2)
    {
        fprintf(stderr, "Use: %s nombre_archivo.extencion\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (stat(argv[1], &sb) == -1)
    {
        perror("stat");
        exit(EXIT_FAILURE);
    }

    printf("Tipo de archivo: ");

    switch (sb.st_mode & S_IFMT)
    {
    case S_IFBLK:
        printf("block device\n");
        break;
    case S_IFCHR:
        printf("character device\n");
        break;
    case S_IFDIR:
        printf("Directorio\n");
        break;
    case S_IFIFO:
        printf("FIFO/pipe\n");
        break;
    case S_IFLNK:
        printf("symlink\n");
        break;
    case S_IFREG:
        printf("Archivo regular\n");
        break;
    case S_IFSOCK:
        printf("socket\n");
        break;
    default:
        printf("unknown?\n");
        break;
    }
    printf("Tamaño archivo: %lld bytes\n", (long long)sb.st_size);

    printf("Ultima modificacion:       %s", ctime(&sb.st_ctime));
    printf("Ultimo acceso:         %s", ctime(&sb.st_atime));
    printf("Ultima modificacion:   %s", ctime(&sb.st_mtime));
    
    ///////////////////////
    time_t t = sb.st_mtime;
    struct tm lt;
    localtime_r(&t, &lt);
    char timbuf[80];
    strftime(timbuf, sizeof(timbuf), "%c", &lt);
    printf("\n\n%s\n\n",timbuf);
    ///////////////////////
    exit(EXIT_SUCCESS);
}