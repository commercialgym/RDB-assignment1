// assignment1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma warning(disable:4996)
#define MAX_CHARS 25
#define ITEM_SIZE (sizeof(int) + NAME_SIZE + CATEGORY_SIZE + sizeof(int) + sizeof(float)) //to be used for accessing the index of a file

typedef struct {
    int productID;
    char name[MAX_CHARS];
    char category[MAX_CHARS];
    int quantity;
    float price;
}StoreItems;

long indexFilePosition(int productID);
long CalculateFileSize(FILE* pFile);
int NumElements(FILE* pFile);

int main(void)
{

    FILE* pFile = fopen("StoreCatalog.dat", "rb+"); //change to rb+ for when actually coding the program
    if (pFile == NULL)
    {
        printf("Unable to open file\n");
        return 1;
    }


    //functionalities---------------

    //open file

    //read specific index

    //edit specific index

    //add product at the end of the file

    //delete a product by marking it as deleted or reusing the space

    //tell user where we are in the file? and maybe add the data of that index

    //when adding a new struct to the file, the product ID is the index of the file + 1
        //calculate the size of the file (with ftell? or fseek?) and turn bytes into index
        //the last product's id in the file is the last index + 1 / it's the total number of elements
        //add 1 for the new product ID

    //functions needed --------------

    //calculate how many elements are in the file
    
    //print struct info

    //create new instance of struct, from user input


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
long CalculateFileSize(FILE* pFile) 
{
    fseek(pFile, 0, SEEK_END); //reminder that this means the fp is placed at the end, should i move it to the beginning?
    long totalSize = ftell(pFile);
    return totalSize;
}

//relies on other file to return proper number of bytes, may combine these two later?
int NumElements(FILE* pFile)
{
    return CalculateFileSize(pFile) / sizeof(StoreItems);
}