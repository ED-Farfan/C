#include <stdio.h>
#include <stdlib.h>
int suma(int a,int b);
int suma(int a,int b,int d);
int main(int argc, char const *argv[])
{
    printf("\n SUMA: %d\n",suma(2,3));
    return 0;
}
int suma(int a,int b){
    return a + b;
}
int suma(int a,int b,int d){
    return a + b + d;
}
