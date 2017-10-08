#include<stdio.h>
#include<stdlib.h>
#include<fstream.h>


int main(int args, char *argv[])
{
    FILE *f;
    char buff;
    int ok =0;
    if(args!=2)
        printf("Numarul parametrilor este invalid!");
    f=fopen(argv[1],"r");
    if(f==NULL)
        printf("Eroare la deschiderea fisierului");
    while(1)
    {
        buff= fgetc(f);
        if(feof(buff)) break;

        ok = at_parser(buff);

    }

       fclose(f);
       
       return 0;
}