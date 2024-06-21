#include <stdio.h>
#include <stdlib.h>
#define YEARS 5
#define MONTHS 12

int main()
{
    const int matrix[YEARS][MONTHS] = {
    {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12},
    {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12},
    {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12},
    {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12},
    {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}
    };

    const char *month[MONTHS] = {"Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio",
        "Julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre"};

    int monthlyAverage[MONTHS] = {0};
    int yearlyAverageArr[YEARS] = {0};
    float yearlyAverage = 0;
    for(int i = 0; i < YEARS; i++)
    {
        int yearTotal = 0;
        for(int j = 0; j < MONTHS; j ++)
        {
            int rain = matrix[i][j];
            yearTotal+= rain;
            monthlyAverage[j] += rain;
        }

        yearlyAverage+= yearTotal;
        yearlyAverageArr[i] = yearTotal;
    }

    yearlyAverage = (float) yearlyAverage / YEARS;
    printf("The average of the five years is: %.2f", yearlyAverage);

    for(int i = 0; i < YEARS; i++)
    {
        float yearAverage = (float) yearlyAverageArr[i] / YEARS;
        printf("\nThe average of the year %d is: %.2f", i, yearAverage);

    }


    for(int i = 0; i < MONTHS; i++)
    {
        float averageMonth = (float) monthlyAverage[i] / YEARS;
        printf("\nThe average of the month %s is: %.2f", month[i], averageMonth);

    }

    return 0;
}
