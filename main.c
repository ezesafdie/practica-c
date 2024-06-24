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

typedef struct
{
    char codigo[TAM_CODIGO];
    int position;
} IndexPos;

int generarProductos(const char* nomArchProd);
int generarMovimientos(const char* nomArchMov);
void mostrarProductos(const char* nomArchProd);
void mostrarMovimientos(const char* nomArchMov);
void actualizarProductos(const char* nomArchProd, const char* nomArchMov);
void guardarNuevoProducto(Movimiento* mov, FILE* temProdFile, FILE* movFile);
void stringCopy(char* stringTo, char* stringFrom);
int stringCompare(char* firstString, char* secondString);
int createIndex(const char* nomArchProd);
void actualizarProductosConIndice(const char* nomArchProd, const char* nomArchMov, const char* nomArchIndex);
int generarMovimientosTest(const char* nomArchMov);


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

    createIndex("test.tmp");


    char string1[10] = "Prueba1";
    char string2[10] = "Eliminar";

    //strcpy(string2, string1);
    stringCopy(string2, string1);
    printf("\n\n%s", string2);


    char string3[10] = "aaa";
    char string4[10] = "aa";

   // printf("\n\n%d", stricmp(string3, string4));
    printf("\n\n%d", stringCompare(string3, string4));

    printf("\n========================\n\n");
    generarMovimientosTest("movimientosTest.tmp");
    mostrarMovimientos("movimientosTest.tmp");
    printf("\n========================\n\n");
    actualizarProductosConIndice("test.tmp", "movimientosTest.tmp", "index.tmp");
    mostrarProductos("test.tmp");


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

int generarMovimientosTest(const char* nomArchMov)
{
    Movimiento movimientos[CANT_MOVS - 3] = {
        {"00", 10},
        {"02", -10},
        {"05", -3},
        {"07", 10},
        {"09", 03},
        {"20", 6},
        {"50", 100}
    };

    FILE* fileMovimientos = fopen(nomArchMov, "wb");
    fwrite(movimientos, sizeof(Movimiento), CANT_MOVS - 3, fileMovimientos);

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

int createIndex(const char* nomArchProd)
{
    FILE* prods = fopen(nomArchProd, "rb");
    if (!prods)
        return -1;

    FILE* index = fopen("index.tmp", "wb");
    if(!index)
        return -1;

    IndexPos indexPos;
    Producto producto;
    fread(&producto, sizeof(Producto), 1, prods);
    while(!feof(prods))
    {
        stringCopy(indexPos.codigo, producto.codigo);

        int position = ftell(prods) - sizeof(Producto);
        indexPos.position = position;

        fwrite(&indexPos, sizeof(IndexPos), 1, index);
        fread(&producto, sizeof(Producto), 1, prods);
    }

    fclose(prods);
    fclose(index);

    return 0;
}


void actualizarProductosConIndice(const char* nomArchProd, const char* nomArchMov, const char* nomArchIndex)
{
    FILE* prodFile = fopen(nomArchProd, "rb+");
    FILE* movFile = fopen(nomArchMov, "rb");
    FILE* indexFile = fopen(nomArchIndex, "rb");

    //FILE* temProdFile = fopen("temProd.tmp", "wb");

    // validar que se abrieron bien

    Producto producto;
    Movimiento movimiento;
    IndexPos indexPos;
    int comp;

    //fread(&producto, sizeof(Producto), 1, prodFile);
    fread(&movimiento, sizeof(Movimiento), 1, movFile);
    fread(&indexPos, sizeof(IndexPos), 1, indexFile);

    while(!feof(indexFile) && !feof(movFile))
    {
        comp = stringCompare(indexPos.codigo, movimiento.codigo);

        if(comp == 0) // Se encontro el codigo del movimiento en el indice.
        {
            fseek(prodFile, indexPos.position, SEEK_SET);
            fread(&producto, sizeof(Producto),  1, prodFile);

            producto.stock += movimiento.cantidad;

            fseek(prodFile, indexPos.position, SEEK_SET);
            fwrite(&producto, sizeof(Producto), 1, prodFile);

            fread(&movimiento, sizeof(Movimiento), 1, movFile); // una vez actualizada. Leemos lo siguiente asi pasamos al siguiente movimiento, pero manteniendo el mismo producto
            fread(&indexPos, sizeof(IndexPos), 1, indexFile);
        }

        if(comp < 0)
        {
            fread(&indexPos, sizeof(IndexPos), 1, indexFile);
        }



    }
    fclose(prodFile);
    fclose(movFile);
    //fclose(temProdFile);
    fclose(indexFile);

  //  remove(nomArchProd);
   // rename("temProd.tmp", nomArchProd);
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
