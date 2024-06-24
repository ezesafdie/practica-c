#include <stdio.h>
#include <stdlib.h>

#define ARG_PROD 1
#define ARG_MOV 2
#define TAM_CODIGO 7 // Es de 6 pero ponemos 7 por el caracter nulo
#define TAM_DESCRIPCION 31
#define CANT_PRODS 5
#define CANT_MOVS 10

#define TODO_OK 0
#define ERR_ARCHIVO 1

#define NOM_ARCH_PROD_TMP "Productos.tmp"

typedef struct
{
    char codigo[TAM_CODIGO];
    char descripcion[TAM_DESCRIPCION];
    int stock;
} Producto;

typedef struct
{
    char codigo[TAM_CODIGO];
    int cantidad;
} Movimiento;

int generarProductos(const char* nomArchProd);
int generarMovimientos(const char* nomArchMov);
void mostrarProductos(const char* nomArchProd);
void mostrarMovimientos(const char* nomArchMov);
void actualizarProductos(const char* nomArchProd, const char* nomArchMov);
void guardarNuevoProducto(Movimiento* mov, FILE* temProdFile, FILE* movFile);
void stringCopy(char* stringTo, char* stringFrom);
int stringCompare(char* firstString, char* secondString);

int main()
{
    generarProductos("test.tmp");
    mostrarProductos("test.tmp");
    printf("========================\n\n");

    generarMovimientos("movimientos.tmp");
    mostrarMovimientos("movimientos.tmp");

    printf("========================\n\n");
    actualizarProductos("test.tmp", "movimientos.tmp");
    mostrarProductos("test.tmp");


    char string1[10] = "Prueba1";
    char string2[10] = "Eliminar";

    //strcpy(string2, string1);
    stringCopy(string2, string1);
    printf("\n\n%s", string2);


    char string3[10] = "aaa";
    char string4[10] = "aa";

   // printf("\n\n%d", stricmp(string3, string4));
    printf("\n\n%d", stringCompare(string3, string4));


    return 0;
}

int generarProductos(const char* nomArchProd)
{
    Producto productos[CANT_PRODS] = {
        {"00", "Higos", 10},
        {"02", "Sandia", 10},
        {"05", "Manzanas", 10},
        {"09", "Mandarina", 10},
        {"20", "Peras", 10}
        //{"21", "Lechuga", 10}
    };

    FILE* fileProductos = fopen(nomArchProd, "wb");
    if(!fileProductos)
        return -1;

    fwrite(productos, sizeof(Producto), CANT_PRODS, fileProductos);

    fclose(fileProductos);

    return  1;
}

int generarMovimientos(const char* nomArchMov)
{
    Movimiento movimientos[CANT_MOVS] = {
        {"00", 10},
        {"00", 15},
        {"02", -10},
        {"05", -3},
        {"07", 10},
        {"09", 03},
        {"20", 6},
        {"20", 8},
        {"20", 4},
        {"50", 100}
    };

    FILE* fileMovimientos = fopen(nomArchMov, "wb");
    fwrite(movimientos, sizeof(Movimiento), CANT_MOVS, fileMovimientos);

    fclose(fileMovimientos);

    return 1;
}

void mostrarProductos(const char* nomArchProd)
{
    FILE* file = fopen(nomArchProd, "rb");

    Producto prod;
    while(fread(&prod, sizeof(Producto), 1, file) > 0)
    {
        printf("Codigo: %s\tDescripcion: %s\tStock: %d\n", prod.codigo, prod.descripcion, prod.stock);
    }

    fclose(file);
}

void mostrarMovimientos(const char* nomArchMov)
{
    FILE* file = fopen(nomArchMov, "rb");

    Movimiento mov;
    while(fread(&mov, sizeof(Movimiento), 1, file) > 0)
    {
        printf("Codigo: %s\tStock: %d\n", mov.codigo, mov.cantidad);
    }

    fclose(file);
}

void actualizarProductos(const char* nomArchProd, const char* nomArchMov)
{
    FILE* prodFile = fopen(nomArchProd, "rb");
    FILE* movFile = fopen(nomArchMov, "rb");

    FILE* temProdFile = fopen("temProd.tmp", "wb");

    // validar que se abrieron bien

    Producto producto;
    Movimiento movimiento;
    int comp;

    fread(&producto, sizeof(Producto), 1, prodFile);
    fread(&movimiento, sizeof(Movimiento), 1, movFile);

    while(!feof(prodFile) && !feof(movFile))
    {
        comp = stringCompare(producto.codigo, movimiento.codigo);

        if(comp == 0) // Si da 0, es que hay que actualizar la cantidad.
        {
            producto.stock += movimiento.cantidad; // actualizamos
            fread(&movimiento, sizeof(Movimiento), 1, movFile); // una vez actualizada. Leemos lo siguiente asi pasamos al siguiente movimiento, pero manteniendo el mismo producto
        }

        if(comp < 0) // Si es - de 0. Es que paso a un codigo que es mayor al producto actual. Se termina de comparar.
        {
            fwrite(&producto, sizeof(Producto), 1, temProdFile);
            fread(&producto, sizeof(Producto), 1, prodFile); // Se lee el siguiente producto para reiniciar el ciclo.
        }

        if(comp > 0) // Es un movimiento que no tiene producto (Producto nuevo)
        {
            guardarNuevoProducto(&movimiento, temProdFile, movFile);
        }

    }

    // por si termino un archivo y no el otro, hay que agregar lo que falta
    while(!feof(prodFile))
    {
        fwrite(&producto, sizeof(Producto), 1, temProdFile);
        fread(&producto, sizeof(Producto), 1, prodFile);
    }

    while(!feof(movFile))
    {
        guardarNuevoProducto(&movimiento, temProdFile, movFile);
    }

    fclose(prodFile);
    fclose(movFile);
    fclose(temProdFile);

    remove(nomArchProd);
    rename("temProd.tmp", nomArchProd);
}

void guardarNuevoProducto(Movimiento* mov, FILE* temProdFile, FILE* movFile)
{
    Producto prod = {"00", "Sin desc", 0};
    stringCopy(prod.codigo, mov->codigo);

    while(!feof(movFile) && stringCompare(prod.codigo, mov->codigo) == 0)
    {
        prod.stock += mov->cantidad;
        fread(mov, sizeof(Movimiento), 1, movFile);
    }

    fwrite(&prod, sizeof(Producto), 1, temProdFile);
}

void stringCopy(char* stringDest, char* stringFrom)
{
    while(*stringFrom)
    {
        *stringDest = *stringFrom;
        stringDest++;
        stringFrom++;
    }
    *stringDest = '\0';
}

int stringCompare(char* firstString, char* secondString)
{
    while(*firstString)
    {
        if(*firstString > *secondString)
            return 1;

        if(*firstString < *secondString)
            return -1;

        firstString++;
        secondString++;
    }

    if(*firstString == '\0' && *secondString == '\0')
        return 0;

    return -1;
}
