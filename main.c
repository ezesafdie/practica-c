#include <stdio.h>
#include <stdlib.h>

int stringLength(const char* string);

int main()
{
    const char string[] = "Cadena de caracteres";
           char *str = "hello world";

    stringLength(string);

    printf("%d", stringLength(str));

    return 0;
}

int stringLength(const char* string)
{
    const char* lastAdress = string;

    while(*lastAdress)
        ++lastAdress; // Al final queda apuntando al caracter nulo "\0"

    return lastAdress - string;
}
