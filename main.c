#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    char* itemName;
    int quantity;
    float price;
    float amount;
} Item;

void readItem(Item* item);
void printItem(Item* item);

int main()
{
    Item* item = malloc(sizeof(Item));
    if(!item)
        return -1;

    item->itemName = (char*) malloc(50 * sizeof(char));
    if(!item->itemName)
        return -1;

    readItem(item);

    printItem(item);


    free(item->itemName);
    free(item);

    return  0;
}

void readItem(Item* item)
{
    printf("Enter the item name: \n");
    scanf("%s", item->itemName);

    printf("\nEnter the quantity: \n");
    scanf("%d", &item->quantity);

    printf("\nEnter the price: \n");
    scanf("%f", &item->price);

    item->amount = item->price * item->quantity;
}

void printItem(Item* item)
{
    printf("Item: %s\nQuantity: %d\nPrice:%.2f\nAmount:%.2f", item->itemName, item->quantity, item->price, item->amount);
}
