#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    char name[50];
    int hireDate;
    float salary;
} Employee;

int main()
{
   Employee employee = {"Juan", 20240622, 200.5};
   Employee secondEmployee;

   printf("Employee name: %s. Date: %d. Salary: %.2f", employee.name, employee.hireDate, employee.salary);
   printf("\nEnter info for another employee\n");
   printf("Name: ");
   scanf(" ");
   gets(secondEmployee.name);
   printf("\nHire Date: ");
   scanf("%d", &secondEmployee.hireDate);
   printf("\nSalary: ");
   scanf("%f", &secondEmployee.salary);


   printf("Employee name: %s. Date: %d. Salary: %.2f", secondEmployee.name, secondEmployee.hireDate, secondEmployee.salary);


   return 0;
}

