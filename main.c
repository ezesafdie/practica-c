#include <stdio.h>
#include <stdlib.h>

int main()
{
    int c;
    int count;

    FILE* file = fopen("file.txt", "r");

    if(!file)
        return -1;

    while((c = fgetc(file)) != EOF)
    {
        if(c == '\n')
            count++;

        printf("%c", c);
    }

    fclose(file);

    printf("\n\nCantidad de lineas: %d", count + 1);

    return  0;
}
