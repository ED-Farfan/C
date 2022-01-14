#include <stdio.h>
#include <string.h>
#include <stdio.h>

int BUFFER_SIZE = 1500;
FILE *source;
FILE *destination;
int n;
int count = 0;
int written = 0;

int main()
{
    unsigned char buffer[BUFFER_SIZE];

    source = fopen("Nueva.png", "rb");

    if (source)
    {
        destination = fopen("rfc1350.png", "wb");
        int min =10;
        int aux;
        while (!feof(source))
        {
            n = fread(buffer, 1, 1, source);            
            count += n;
            
            for (int i = 0; i < 1; i++)
            {
                printf("\n Caracter: %c Codigo: %d",buffer[i],buffer[i]);
                aux = buffer[i];
                if(aux < min){
                    min =aux;
                }
            }
            
            
            fwrite(buffer, 1, n, destination);
        }
        buffer[0] = '\0';
        printf("%d bytes read from library.\nMinimo: %d\n", count,min);
    }
    else
    {
        printf("fail\n");
    }

    fclose(source);
    fclose(destination);

    return 0;
}
