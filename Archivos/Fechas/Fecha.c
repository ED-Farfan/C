#include <time.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
void Concerir_Char_a_Time_t();
void modificarFechaDeModificacion(char file[]);
char *formatdate(char *str, time_t val)
{
    strftime(str, 36, "%d.%m.%Y %H:%M:%S", localtime(&val));
    return str;
}

int main()
{
    int errno;
    const char *filename;
    filename = "Hola 23";

    errno = 0;
    struct stat *file_info = malloc(sizeof(struct stat));
    if (lstat(filename, file_info) != 0)
    {
        perror("Error");
        exit(1);
    }

    char date[36];
    printf("Access: %s\n", formatdate(date, file_info->st_atime));
    printf("Modify: %s\n", formatdate(date, file_info->st_mtime));
    printf("Change: %s\n", formatdate(date, file_info->st_ctime));
    free(file_info);

    Concerir_Char_a_Time_t();
    //modificarFechaDeModificacion("Nueva.png");
    modificarFechaDeModificacion("Hola\\ 23");
    return 0;
}

void Concerir_Char_a_Time_t()
{
    char date[] = "2011-04-01";
    char fech[50];
    date[4] = date[7] = '\0';
    struct tm tmdate = {0};
    tmdate.tm_year = atoi(&date[0]) - 1900;
    tmdate.tm_mon = atoi(&date[5]) - 1;
    tmdate.tm_mday = atoi(&date[8]);
    tmdate.tm_hour=13;
    tmdate.tm_min=34;
    tmdate.tm_sec=32;
    time_t t = mktime(&tmdate);
    printf("HOLA: %s\n", formatdate(fech, t));
    //printf("Nose %d %d %d\n", atoi(&date[0]),atoi(&date[5]),atoi(&date[8]));
}

void modificarFechaDeModificacion(char file[]){
    char command[256];
    snprintf(command, sizeof command, "touch   -t 201510021330.13 %s", file);
    system(command);
}