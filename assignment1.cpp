// assignment1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma warning(disable:4996)
#define MAX_CHARS 25
#define ITEM_SIZE (sizeof(int) + NAME_SIZE + CATEGORY_SIZE + sizeof(int) + sizeof(float)) //to be used for accessing the index of a file

long indexFilePosition(int productID);

int main(void)
{
    typedef struct {
        int productID;
        char name[MAX_CHARS];
        char category[MAX_CHARS];
        int quantity;
        float price;
    }StoreItems;

    FILE* pFile = fopen("StoreCatalog.dat", "wb+"); //change to rb+ for when actually coding the program
    if (pFile == NULL)
    {
        printf("Unable to open file\n");
        return 1;
    }

    //might write an arbitrary amount of data to the file initially to use later for read, update, delete

    StoreItems fileData = { 1, "Smartphone", "Electronics", 50, 899.99 };
    StoreItems fileData2 = { 2, "Smartphone", "Electronics", 30, 799.99 };
    StoreItems fileData3 = { 3, "Laptop", "Electronics", 30, 599.99 };
    StoreItems fileData4 = { 4, "Apples", "Produce", 110, 4.99 };


    fwrite(&fileData, sizeof(StoreItems), 1, pFile);
    fwrite(&fileData2, sizeof(StoreItems), 1, pFile);
    fwrite(&fileData3, sizeof(StoreItems), 1, pFile);
    fwrite(&fileData4, sizeof(StoreItems), 1, pFile);

    StoreItems readItem;
    fseek(pFile, sizeof(StoreItems) * 2, SEEK_SET);
    fread(&readItem, sizeof(StoreItems), 1, pFile);
    printf("%d, %s, %s, %d, %f\n", readItem.productID, readItem.name, readItem.category, readItem.quantity, readItem.price);

    if (fclose(pFile) == EOF) {
        printf("Error closing file\n\n");
        clearerr(pFile);
    }

    return 0;
}

//NAME:
//PARAMETER:
//DESCRIPTION:
//RETURNS: 
long indexFilePosition(int productID)
{
    return 0;
}

//NAME:
//PARAMETER:
//DESCRIPTION:
//RETURNS: 
