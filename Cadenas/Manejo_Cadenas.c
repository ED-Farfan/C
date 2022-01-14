#include <stdio.h>
#include <stdlib.h>

char nom1[] = "Hola mundo";
char *nom2 = NULL;
int TAM;
void Copiar_cadenas(char *a, char **r);
void Multiplicar_cadena(char **a, int m);
int Tamanio_Char(char *c);
void EnteroACaracter(char **cad, int numero);
void Contatenar_Cadenas(char *a, char *b, char **r);

int main()
{
    char *b = NULL;
    //EnteroACaracter(&b,12345678);
    Contatenar_Cadenas("1234", "xyz", &b);
    Copiar_cadenas(b, &nom2);
    printf("\n\n -%s-\n", nom2);
    Multiplicar_cadena(&nom2, 4);
    printf("\n\n -%s-\n", nom2);

    return 0;
}
int Tamanio_Char(char *c)
{
    if (c != NULL)
    {
        int i = 0;
        while (c[i] != '\0')
        {
            i++;
        }
        return i;
    }
    else
    {
        return 0;
    }
}
void Copiar_cadenas(char *a, char **r)
{

    if (a != NULL && *r == NULL)
    {
        char *res = NULL;
        int longitud = Tamanio_Char(a);
        res = (char *)malloc((longitud + 1) * sizeof(char));
        for (int i = 0; i < longitud; i++)
        {
            res[i] = a[i];
        }
        res[longitud] = '\0';
        *r = res;
    }
}
void Multiplicar_cadena(char **a, int m)
{
    if (*a != NULL && m >= 2)
    {
        char *u = *a;
        char *res = NULL;
        int longitud = Tamanio_Char(*a);
        res = (char *)malloc((longitud) * sizeof(char) * m + 1);
        int apun = 0;
        int max = ((longitud) * sizeof(char) * m + 1);
        for (int j = 0; j < m; j++)
        {
            for (int i = 0; i < longitud; i++)
            {
                res[apun] = u[i];
                apun++;
            }
        }
        res[max] = '\0';
        *a = res;
    }
    else if (*a != NULL && m == -1)
    {
        char *u = *a;
        char *res = NULL;
        int longitud = Tamanio_Char(*a);
        res = (char *)malloc((longitud) * sizeof(char) + 1);

        int max = ((longitud) * sizeof(char) + 1);
        int apun = max - 2;
        for (int i = 0; i < longitud; i++)
        {
            res[apun] = u[i];
            apun--;
        }

        res[max] = '\0';
        *a = res;
    }
}
void EnteroACaracter(char **cad, int numero)
{

    if (*cad == NULL)
    {
        int cont = 0;
        int div = numero, res = 0;
        while (div != 0)
        {
            res = div % 10;
            div = div / 10;
            cont++;
        }
        div = numero;
        char *cadn = NULL;
        cadn = (char *)malloc((cont) * sizeof(char) + 1);
        for (int i = 0; i < cont; i++)
        {
            res = div % 10;
            div = div / 10;
            cadn[i] = res + '0';
        }
        cadn[cont] = '\0';
        Multiplicar_cadena(&cadn, -1);
        *cad = cadn;
    }
}
void Contatenar_Cadenas(char *a, char *b, char **r)
{

    if (a != NULL && b != NULL && *r == NULL)
    {
        int tam_a = Tamanio_Char(a);
        int tam_b = Tamanio_Char(b);
        char *res = (char *)malloc((tam_a + tam_b) * sizeof(char) + 1);
        int apuntador = 0;
        for (int i = 0; i < tam_a; i++)
        {
            res[i] = a[i];
            apuntador++;
        }
        for (int i = 0; i < tam_b; i++)
        {
            res[apuntador] = b[i];
            apuntador++;
        }
        res[tam_a + tam_b] = '\0';
        *r = res;
    }
}