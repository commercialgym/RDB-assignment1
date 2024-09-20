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

    FILE* pFile = fopen("StoreCatalog", "wb+");
    if (pFile == NULL)
    {
        printf("Unable to open file\n");
        return 1;
    }



    return 0;
}

//NAME:
//PARAMETER:
//DESCRIPTION:
//RETURNS: 
long indexFilePosition(int productID)
{

}

//NAME:
//PARAMETER:
//DESCRIPTION:
//RETURNS: 