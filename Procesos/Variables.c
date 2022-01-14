#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
    pid_t pid;
    int num_1=45, num_2= 5;
    int suma, resta;
    int estado;
    printf("\nProceso Padre\n");
    pid = fork();
    if (pid == -1)
    {
        perror("\n Error al crear el proceso hijo\n");
        exit(EXIT_FAILURE);
    }
    if(pid == 0)//if (!pid)
    {
        printf("\n\tProceso hijo con pid: %d \n",getpid());
        suma = num_1 + num_2;
        printf("\nLa suma es: %d\n",suma);
        exit(1);//Se multiplica por 256
    }else{
        printf("Proceso padre con pid: %d \n",getpid());
        resta = num_1 - num_2;
        printf("\nLa resta es: %d\n",resta);
        pid = wait(&estado);
        printf("\nTermino el proceso hijo con el pid: %d y don estado: %d \n", pid,estado);
    }
    return 0;
}
