#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    pid_t pid;
    printf("\nProceso Padre\n");
    pid = fork();
    if (pid == -1)
    {
        perror("\n Error al crear el proceso hijo\n");
        exit(EXIT_FAILURE);
    }
    if (!pid)
    {
        printf("\n\tProceso hijo con pid: %d \n",getpid());
        exit(0);
    }else{
        printf("Proceso padre con pid: %d \n",getpid());
    }
    return 0;
}
