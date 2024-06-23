#include <stdio.h>
#include <stdlib.h>

int isLowerCase(char c);

int main()
{
    int i;
    int c;
    int count = 0;

    FILE* file = fopen("file.txt", "rb");
    FILE* fileToWrite = fopen("temp.txt", "w+");

    if(!file)
        return -1;

    if(!fileToWrite)
    {
        fclose(file);
        return -1;
    }

    fseek(file, -1, SEEK_END);

    count = ftell(file);

    while(i <= count)
    {
        c = fgetc(file);
        fputc(c, fileToWrite);
        fseek(file, -2, SEEK_CUR);

        i++;
    }

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
