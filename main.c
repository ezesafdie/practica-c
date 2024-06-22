#include <stdio.h>
#include <stdlib.h>

int main()
{
    int size;
    char *text = NULL;

    printf("Enter the amount of characters the string will have:\n");
    scanf("%d", &size);

    text = (char *) malloc(size * sizeof(char));
    if(!text)
        return -1;

    printf("Enter the string:\n");
    scanf(" ");
    gets(text);

    printf("Lo ingresado fue: %s", text);

    free(text);
    return 0;
}

