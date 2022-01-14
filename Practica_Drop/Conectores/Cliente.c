#include "conexion.h"

int main(int argc, char const *argv[])
{
    printf("\nRegreso%d\n",Mandar_Archivos("192.168.1.73",8181, "Prueba.txt"));
    return 0;
}
