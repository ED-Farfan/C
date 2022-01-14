#include <stdio.h>
#include <stdlib.h>
void LeerArchivo();
void LeerArchivo2();
void LeerArchivo3();
void LeerArchivo4();
void Escribir();

int main(int argc, char const *argv[])
{
    LeerArchivo();
    LeerArchivo2();
    LeerArchivo3();
    LeerArchivo4();
    Escribir();
    return 0;
}
void LeerArchivo(){ /// Lee un caracter de más lo arreglamos en la condicion if
    FILE * f = fopen("BD/datos.txt","rb");
    if(f == NULL){
        printf("\nError en la apertura de archivo\n");
    }else{
        char c;
        while(feof(f)==0){
            c = fgetc(f);
            if(c!='\0' && (int)c!=-1){
                printf("%c",c);
            }            
        }
        fclose(f);
        printf("\n");
    }
}
void LeerArchivo2(){
    FILE * f = fopen("BD/datos.txt","rb");
    if(f == NULL){
        printf("\nError en la apertura de archivo\n");
    }else{
        char c;
        while(feof(f)==0){
            c = fgetc(f);
            if(c!='\0'&&(int)c!=-1){
                printf("%c",c);
                printf(" %d",(int)c);
                printf("\n");
            }            
        }
        fclose(f);
        printf("\n");
    }
}
void LeerArchivo3(){
    FILE * f = fopen("BD/datos.txt","rb");
    if(f == NULL){
        printf("\nError en la apertura de archivo\n");
    }else{
        char c[100];
        while(feof(f)==0){
            fscanf(f,"%s",c); //Quita los espacios
            printf("%s",c);
        }
        fclose(f);
        printf("\n");
    }
}
void LeerArchivo4(){
    FILE * f = fopen("BD/datos2.txt","rb");
    if(f == NULL){
        printf("\nError en la apertura de archivo\n");
    }else{
        char c[100];
        char s[100];
        int a =0;
        while(feof(f)==0){
            fscanf(f,"%s%d%s",c,&a,s);
            printf("\n%s %d %s",c,a,s);
            printf("\n%s %d %s\n",c,a*10,s);
        }
        fclose(f);
        printf("\n");
    }
}
void Escribir(){
    FILE * f = fopen("BD/datos3.txt","a"); // 'a' para escribir despues de lo guardado , "w" para escribir y borrar todo
    if(f == NULL){
        printf("\nError en la apertura de archivo\n");
    }else{
        /*
            fputc - escribe un caracter
            fputs - escribe una cadena de texto
            fprintf - equivalente a printf
        */
        char sas[] = "Hola a todos";
        for (int i = 0; i < 10; i++)
        {
            for (int j = 0; j < 10; j++)
            {
                fputc('*',f);
                fputc('_',f);                
            }
            fputs(" Hola perros",f);
            //fprintf("%s %i",sas,i);
            fputc('\n',f);
        }

        fflush(f);        

        fclose(f);
        printf("\n");
    }
}