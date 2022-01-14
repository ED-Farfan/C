#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
//#include <cstdlib>
typedef struct THilo //Trabajo De Hilo
{
    int FilaI;
    int FilaF;
    int hilo;
    int **MatrizA;
    int **MatrizB;
    int **MatrizC;
    int columnaC;
    int columnaA;
} THilo;

int **MatrizEnteros(int columnas, int filas);
int **MatrizCeros(int columnas, int filas);
void ImprimirMatriz(int columnas, int filas, int **matriz, char Matriz[]);
int **EliminarMatriz(int columnas, int filas, int **matriz);
int **MatrizEnterosT(int columnas, int filas, int **matriz);
void MultiplicarRenglonDeMatrices(int **MatA, int **MatB, int **MatC, int Fila, int Columnas, int Filas);
void Dimencionar_NuevaMatriz(int filasA, int columnaA, int filasB, int columnaB, int *(filasC), int *(columnaC));
void DatosMatriz(int *filas, int *columnas, char matriz[]);
int NumeroDeHilos(int filasC);
THilo *Arreglo(int hilos);
void AsignacionDeTrabajo(THilo *raiz, int hilos, int FilasC, int **MA, int **MB, int **MC, int columnaC, int columnaA);
void *function(void *parametro);

int main(int argc, char const *argv[]) // Podemos mandar  Fila de A , Columna de A ,Fila de B , Columna de B,Hilos
{
    if (argc == 1)
    {

        int filasA = 0, columnaA = 0;
        int filasB = 0, columnaB = 0;
        int filasBT = columnaB, columnaBT = filasB;
        int filasC = 0, columnaC = 0;
        THilo *raiz = NULL;
        int **matrizC = NULL;
        DatosMatriz(&filasA, &columnaA, "A");
        DatosMatriz(&filasB, &columnaB, "B");

        Dimencionar_NuevaMatriz(filasA, columnaA, filasB, columnaB, &filasC, &columnaC);
        if (filasC >= 1)
        {
            int **matrizA = MatrizEnteros(columnaA, filasA);

            int **matrizB = MatrizEnteros(columnaB, filasB);

            int **matrizBT = MatrizEnterosT(columnaB, filasB, matrizB);

            matrizC = MatrizCeros(columnaC, filasC);

            /*for (int i = 0; i < filasC; i++)
        {
            MultiplicarRenglonDeMatrices(matrizA, matrizBT, matrizC, i, columnaC, columnaA);
        }*/
            int hilos = NumeroDeHilos(filasC);

            pthread_t *thread = NULL;
            thread = (pthread_t *)malloc((hilos) * sizeof(pthread_t));

            printf("\n Numero De Hilos:%d \n", hilos);
            raiz = Arreglo(hilos);
            AsignacionDeTrabajo(raiz, hilos, filasC, matrizA, matrizBT, matrizC, columnaC, columnaA);
            printf("\n\n");
            printf("\nReporte de hilos:\n\n");

            for (int i = 0; i < hilos; i++)
            {
                pthread_create(&thread[i], NULL, function, &raiz[i]);
            }
            for (int i = 0; i < hilos; i++)
            {
                pthread_join(thread[i], NULL);
            }
            printf("\n\n");
            printf("\n\tMatrices\n");
            printf("\nMatriz A\n");
            ImprimirMatriz(columnaA, filasA, matrizA, "A");
            printf("\nMatriz B\n");
            ImprimirMatriz(columnaB, filasB, matrizB, "B");
            printf("\nMatriz C\n");
            ImprimirMatriz(columnaC, filasC, matrizC, "C");

            matrizB = EliminarMatriz(columnaB, filasB, matrizB);
            matrizA = EliminarMatriz(columnaA, filasA, matrizA);
            matrizC = EliminarMatriz(columnaC, filasC, matrizC);
            //free(thread);
            //free(raiz);
        }
        else
        {
            printf("\n¡¡ERROR!! \n");
            printf("\n El Numero De Columnas De La Matriz A Tiene Que Ser El Mismo Numero De Filas De La Matriz B");
            printf("\n\t Columnas De A %d != Filas De B %d\n\n", columnaA, filasB);
        }
    }
    else if (argc == 6)
    {
        int filasA = atoi(argv[1]), columnaA = atoi(argv[2]);
        int filasB = atoi(argv[3]), columnaB = atoi(argv[4]);
        int filasBT = columnaB, columnaBT = filasB;
        int filasC = 0, columnaC = 0;
        THilo *raiz = NULL;
        int **matrizC = NULL;
        Dimencionar_NuevaMatriz(filasA, columnaA, filasB, columnaB, &filasC, &columnaC);
        system("clear");
        if (filasC >= 1 && atoi(argv[5]) <= filasC)
        {
            int **matrizA = MatrizEnteros(columnaA, filasA);

            int **matrizB = MatrizEnteros(columnaB, filasB);

            int **matrizBT = MatrizEnterosT(columnaB, filasB, matrizB);

            matrizC = MatrizCeros(columnaC, filasC);
            int hilos = atoi(argv[5]);
            pthread_t *thread = NULL;
            thread = (pthread_t *)malloc((hilos) * sizeof(pthread_t));
            printf("\n Numero De Hilos:%d \n", hilos);
            raiz = Arreglo(hilos);
            AsignacionDeTrabajo(raiz, hilos, filasC, matrizA, matrizBT, matrizC, columnaC, columnaA);
            printf("\n\n");
            printf("\nReporte de hilos:\n\n");
            for (int i = 0; i < hilos; i++)
            {
                pthread_create(&thread[i], NULL, function, &raiz[i]);
            }
            for (int i = 0; i < hilos; i++)
            {
                pthread_join(thread[i], NULL);
            }
            printf("\n\n");
            printf("\n\tMatrices\n");
            printf("\nMatriz A\n");
            ImprimirMatriz(columnaA, filasA, matrizA, "A");
            printf("\nMatriz B\n");
            ImprimirMatriz(columnaB, filasB, matrizB, "B");
            printf("\nMatriz C\n");
            ImprimirMatriz(columnaC, filasC, matrizC, "C");

            matrizB = EliminarMatriz(columnaB, filasB, matrizB);
            matrizA = EliminarMatriz(columnaA, filasA, matrizA);
            matrizC = EliminarMatriz(columnaC, filasC, matrizC);
            //free(thread);
            //free(raiz);
        }
        else
        {
            if (atoi(argv[3]) <= filasC)
            {
                printf("\n¡¡ERROR!! \n");
                printf("\n El Numero Hilos debe de ser menor o igual al numero de filas de la matriz C.\n ->Filas de C: %d\n -> Numero de Hilos:%d", filasC, atoi(argv[5]));
            }
            else
            {
                printf("\n¡¡ERROR!! \n");
                printf("\n El Numero De Columnas De La Matriz A Tiene Que Ser El Mismo Numero De Filas De La Matriz B");
                printf("\n\t Columnas De A %d != Filas De B %d\n\n", columnaA, filasB);
            }
        }
    }
    else
    {
        system("clear");
        printf("\n¡¡ERROR!! \n");
        printf("\n Argumentos ingresados: %d ", argc);
        printf("\n Favor de ingresar sin argumentos al ejecutar o en los argumentos poner Filas A, Columnas A, Filas B, Columnas B, hilos \n");
    }

    return 0;
}

int **MatrizEnteros(int columnas, int filas)
{
    int **matriz = NULL;
    matriz = (int **)malloc(filas * sizeof(int *));
    for (int i = 0; i < filas; i++)
    {
        matriz[i] = NULL;
        matriz[i] = (int *)malloc(columnas * sizeof(int));
        for (int j = 0; j < columnas; j++)
        {
            matriz[i][j] = rand() % (6);
        }
    }
    return matriz;
}
int **MatrizCeros(int columnas, int filas)
{
    int **matriz = NULL;
    matriz = (int **)malloc(filas * sizeof(int *));
    for (int i = 0; i < filas; i++)
    {
        matriz[i] = NULL;
        matriz[i] = (int *)malloc(columnas * sizeof(int));
        for (int j = 0; j < columnas; j++)
        {
            matriz[i][j] = 0;
        }
    }
    return matriz;
}
void ImprimirMatriz(int columnas, int filas, int **matriz, char Matriz[])
{

    if (columnas <= 10 && filas <= 10)
    {
        for (int i = 0; i < filas; i++)
        {
            for (int j = 0; j < columnas; j++)
            {
                printf("%d", matriz[i][j]);
                printf(" ");
            }
            printf("\n");
        }
    }else
    {
        printf("\nDimensiones de la matriz %d x %d\n",filas,columnas);
        printf("\n\tFilas:%d Columnas: %d\n",filas,columnas);
    }
    
}
int **EliminarMatriz(int columnas, int filas, int **matriz)
{

    for (int i = 0; i < filas; i++)
    {
        free(matriz[i]);
    }
    free(matriz);
    return NULL;
}
int **MatrizEnterosT(int columnas, int filas, int **matriz)
{
    int **matrizT = NULL;
    matrizT = (int **)malloc(columnas * sizeof(int *));
    for (int i = 0; i < columnas; i++)
    {
        matrizT[i] = NULL;
        matrizT[i] = (int *)malloc(filas * sizeof(int));
        for (int j = 0; j < filas; j++)
        {
            matrizT[i][j] = matriz[j][i];
        }
    }
    return matrizT;
}
void MultiplicarRenglonDeMatrices(int **MatA, int **MatB, int **MatC, int Fila, int Columnas, int columnasA)
{

    for (int i = 0; i < Columnas; i++)
    {
        for (int j = 0; j < columnasA; j++)
        {
            //printf("\n Voy C[%d][%d] , MA[%d][%d] ,MBT[%d][%d]",Fila,i,Fila,j,i,j);
            MatC[Fila][i] += MatA[Fila][j] * MatB[i][j];
            //printf("\n C[%d][%d] = %d, MA[%d][%d]= %d ,MBT[%d][%d]=%d, MA*MBT=%d",Fila,i,MatC[Fila][i],Fila,j,MatA[Fila][j],i,j,MatB[i][j],MatA[Fila][j] * MatB[i][j]);
        }
    }
}
/*Comprobacion de multiplicacion*/
void Dimencionar_NuevaMatriz(int filasA, int columnaA, int filasB, int columnaB, int *(filasC), int *(columnaC))
{
    if (columnaA == filasB)
    {
        *filasC = filasA;
        *columnaC = columnaB;
    }
    else
    {
        *filasC = -1;
        *columnaC = -1;
    }
}
/*Pedir datos para la matriz*/
void DatosMatriz(int *filas, int *columnas, char matriz[])
{
    system("clear");
    printf("\t\tIngrese el tamaño de la Fila de la matriz %s \n", matriz);
    printf("->Filas: ");
    scanf("%d", filas);
    printf("\n\t\tIngrese el tamaño de la Columnas de la matriz %s \n", matriz);
    printf("->Columnas: ");
    scanf("%d", columnas);
    system("clear");
}
/*Pedir Numero De Hilos*/
int NumeroDeHilos(int filasC)
{
    int hilos = -1;
    while (hilos < 1 || hilos > filasC)
    {
        system("clear");
        printf("\n\t\t (0 < Numero de hilos <= %d) ", filasC);
        printf("\n\tIngrese el numero de Hilos: ");
        scanf("%d", &hilos);
        system("clear");
    }
    return hilos;
}
THilo *Arreglo(int hilos)
{
    THilo *arreglo = NULL;
    arreglo = (THilo *)malloc(sizeof(THilo) * hilos);
    return arreglo;
}
void AsignacionDeTrabajo(THilo *raiz, int hilos, int FilasC, int **MA, int **MB, int **MC, int columnaC, int columnaA)
{
    int todos = FilasC / hilos;
    int residuo = FilasC - (todos * hilos);
    int InicioS = 0;
    printf("\nTodos realizan %d\n", todos);
    for (int i = 0; i < hilos; i++)
    {
        raiz[i].hilo = i + 1;
        raiz[i].FilaI = InicioS;
        raiz[i].FilaF = (residuo == 0 ? -1 : 0) + raiz[i].FilaI + todos;
        InicioS = raiz[i].FilaF + 1;
        residuo += (residuo == 0 ? 0 : -1);
        raiz[i].MatrizA = MA;
        raiz[i].MatrizB = MB;
        raiz[i].MatrizC = MC;
        raiz[i].columnaA = columnaA;
        raiz[i].columnaC = columnaC;
    }

    for (int i = 0; i < hilos; i++)
    {
        printf("\n\tHilo [%d]", raiz[i].hilo);
        printf("\n Inicia en la Fila: %d", raiz[i].FilaI);
        printf("\n Termina en la Fila: %d\n", raiz[i].FilaF);
    }
}
void *function(void *parametro)
{
    THilo *p = ((THilo *)parametro);
    printf("\nInicio Hilo[%d]\n", p->hilo);
    if (p->hilo > 0)
    {
        for (int i = p->FilaI; i <= p->FilaF; i++)
        {
            MultiplicarRenglonDeMatrices(p->MatrizA, p->MatrizB, p->MatrizC, i, p->columnaC, p->columnaA);
        }
        printf("\nTermino Hilo[%d]\n", p->hilo);
    }

    //pthread_exit(NULL);
    pthread_exit(NULL);
}