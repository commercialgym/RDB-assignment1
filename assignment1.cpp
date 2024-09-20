// assignment1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma warning(disable:4996)
#define MAX_CHARS 26
#define VALID_INPUT 1
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
StoreItems* ReadDataFromID(FILE* pFile, int productID);
bool AddNewProduct(char newName[], char newCategory[], int newQuantity, float newPrice);
StoreItems GetNewProductInfo(void); //from user
void PrintProductInfo(StoreItems* product);

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

    /*
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
    */

    //TESTING
    StoreItems newItem = GetNewProductInfo();
    PrintProductInfo(&newItem);
    

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

//NAME:
//PARAMETER:
//DESCRIPTION:
//RETURNS: 
//relies on other file to return proper number of bytes, may combine these two later?
int NumElements(FILE* pFile)
{
    return CalculateFileSize(pFile) / sizeof(StoreItems);
}

//NAME:
//PARAMETER:
//DESCRIPTION:
//RETURNS: 
StoreItems* ReadDataFromID(FILE* pFile, int productID)
{
    if (productID <= 0 || productID > NumElements(pFile))
    {
        printf("Invalid Product ID, Please Enter within Range 1 - %d\n\n", NumElements(pFile)); //should i put errors in main function?
        return NULL;
    }

    int fileIndex = (productID - 1) * sizeof(StoreItems);
    fseek(pFile, fileIndex, SEEK_SET);
    StoreItems readItem; 
    fread(&readItem, sizeof(StoreItems), 1, pFile);
    return &readItem;
}

bool AddNewProduct(char newName[], char newCategory[], int newQuantity, float newPrice)
{

    return true;
}

//NAME:
//PARAMETER:
//DESCRIPTION:
//RETURNS: StoreItems by value
StoreItems GetNewProductInfo(void)
{
    char newName[MAX_CHARS] = { 0 };
    char newCategory[MAX_CHARS] = { 0 };
    int newQuantity = 0;
    float newPrice = 0;
    printf("Enter the name of the new product: ");
    while (true) //user cannnot exit this should i make an option to do so?
    {
        if (scanf("%25s", newName) != VALID_INPUT)
        {
            printf("Invalid Input: Please enter a name 25 characters or less\n"); //test that it only accepts 25 chars
            printf("Input: ");
        }
        else
        {
            break;
        }
    }
    printf("Enter the category of the new product: ");
    while (true)
    {
        if (scanf("%25s", newCategory) != VALID_INPUT)
        {
            printf("Invalid Input: Please enter a name 25 characters or less\n"); //test that it only accepts 25 chars
            printf("Input: ");
        }
        else
        {
            break;
        }
    }
    printf("Enter the quantity: ");
    while (true)
    {
        if (scanf("%d", &newQuantity) != VALID_INPUT)
        {
            printf("Invalid Input: Please enter an integer value\n");
            printf("Input: ");
        }
        else
        {
            break;
        }
    }
    printf("Enter the price: ");
    while (true)
    {
        if (scanf("%f", &newPrice) != VALID_INPUT)
        {
            printf("Invalid Input: Please enter a float value\n");
            printf("Input: ");
        }
        else
        {
            break;
        }
    }
    //add to struct
    StoreItems newProduct;
    newProduct.productID = 0;
    strcpy(newProduct.name, newName);
    strcpy(newProduct.category, newCategory);
    newProduct.quantity = newQuantity;
    newProduct.price = newPrice;
    //printf("%d, %s, %s, %d, %f\n", newProduct.productID, newProduct.name, newProduct.category, newProduct.quantity, newProduct.price);
    return newProduct;
}

//NAME:
//PARAMETER:
//DESCRIPTION:
//RETURNS: 
void PrintProductInfo(StoreItems* product)
{
    printf("%d, %s, %s, %d, %f\n", product->productID, product->name, product->category, product->quantity, product->price);
}
