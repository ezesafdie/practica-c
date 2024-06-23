#include <stdio.h>
#include <stdlib.h>

int isLowerCase(char c);

int main()
{
    int c;
    int count;

    FILE* file = fopen("file.txt", "r");
    FILE* fileToWrite = fopen("temp.txt", "w+");

    if(!file)
        return -1;

    if(!fileToWrite)
    {
        fclose(file);
        return -1;
    }

    while((c = fgetc(file)) != EOF)
    {
        if(c == '\n')
            count++;

        printf("%c", c);

        if(isLowerCase(c))
        {
            fputc((int)c - 32, fileToWrite);
        } else
        {
            fputc(c, fileToWrite);
        }
    }

    fclose(file);

    printf("\n\nCantidad de lineas: %d", count + 1);

    return  0;
}

int isLowerCase(char c)
{
    if((int) c >= 97 && (int) c <= 122)
    {
        return 1;
    }

    return 0;
}
