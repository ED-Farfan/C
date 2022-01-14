#include <stdio.h>
#include <stdlib.h>
#include "structura.h"

int main(int argc, char const *argv[])
{
    Nodo a[10];
    for (register int i = 0; i < 10; i++)
    {
        a[i].x=i*2;
        a[i].y=i*2+1;
    }
    for (register int i = 0; i < 10; i++)
    {
        printf("\n a[%d]\n\tPar:%d\n\tImpar%d\n",i,a[i].x,a[i].y);
    }


    return 0;
}
