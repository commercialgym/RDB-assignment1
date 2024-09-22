// sequential.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#pragma warning(disable:4996)
#define MAX_CHARS 26
#define VALID_INPUT 1
#define SCND_DECIMAL 100
#define BUFF_SIZE 100

typedef struct {
	int productID;
	char name[MAX_CHARS];
	char category[MAX_CHARS];
	int quantity;
	float price;
}StoreItems;

long CalculateFileSize(FILE* pFile);
int NumElements(FILE* pFile);
bool ReadDataFromFile(FILE* pFile, StoreItems **productCatalog, int count);
void PrintDataFromMemory(StoreItems** productCatalog, int counter);
void AssignProductInfo(StoreItems* newProduct); //from user
void AddProductToList(StoreItems** productCatalog, StoreItems* newProduct, int* numProduct); //adds to list in memory

int main(void)
{
	StoreItems* productCatalog = NULL;

	FILE* pFile = fopen("StoreCatalog.dat", "rb+");
	if (pFile == NULL)
	{
		printf("Unable to open file\n");
		return 1;
	}

	int numProducts = NumElements(pFile);

	//read the data from the file sequentially
	ReadDataFromFile(pFile, &productCatalog, numProducts);

	if (fclose(pFile) == EOF) {
		printf("Error closing file\n\n");
		clearerr(pFile);
	}

		//and print the data that is now held in memory
	PrintDataFromMemory(&productCatalog, numProducts);

	//update the data and write the entire set back to the file in sequence
		//maybe take user input?

		//add a new product
	StoreItems newProduct = { 0, "Deleted", "Deleted", 0, 0 };
	AssignProductInfo(&newProduct);


		//update a product

		//delete a product

	/*
	FILE* pFile = fopen("StoreCatalog.dat", "wb");
	if (pFile == NULL)
	{
		printf("Unable to open file\n");
		return 1;
	}

	//now write the updated product info back to the file


	if (fclose(pFile) == EOF) {
		printf("Error closing file\n\n");
		clearerr(pFile);
	}
	*/

	return 0;
}

//NAME:
//PARAMETER:
//DESCRIPTION:
//RETURNS: 
long CalculateFileSize(FILE* pFile)
{
	long currentPos = ftell(pFile); //capture the current fp position
	fseek(pFile, 0, SEEK_END); //reminder that this means the fp is placed at the end, should i move it to the beginning?
	long totalSize = ftell(pFile);
	fseek(pFile, currentPos, SEEK_SET); //return the fp position to the previous
	return totalSize;
}

//NAME:
//PARAMETER:
//DESCRIPTION:
//RETURNS: 
int NumElements(FILE* pFile)
{
	return CalculateFileSize(pFile) / sizeof(StoreItems);
}

bool ReadDataFromFile(FILE* pFile, StoreItems **productCatalog, int count)
{
	*productCatalog = (StoreItems*)malloc(count * sizeof(StoreItems));
	if (*productCatalog == NULL)
	{
		printf("Error Allocating Memory\n");
		return false;
	}
	
	int index = 0;
	while (index < count)
	{
		if (fread(&(*productCatalog)[index], sizeof(StoreItems), 1, pFile) != VALID_INPUT)
		{
			printf("Error Reading From File\n");
			return false;
		}
		printf("Successful Read: %d\n", index);
		index++;
	}
	return true;
}

//NAME:
//PARAMETER:
//DESCRIPTION:
//RETURNS: 
void PrintDataFromMemory(StoreItems** productCatalog, int counter)
{
	printf("Product Catalog From File:");
	for (int index = 0; index < counter; index++)
	{	
		printf("\nProduct ID: %d\n", (*productCatalog)[index].productID);
		printf("Name: %s\n", (*productCatalog)[index].name);
		printf("Category: %s\n", (*productCatalog)[index].category);
		printf("Quantity: %d\n", (*productCatalog)[index].quantity);
		printf("Price: %.2f\n", (*productCatalog)[index].price);
	}
}

void AssignProductInfo(StoreItems* newProduct)
{
	char newName[MAX_CHARS] = { 0 };
	char newCategory[MAX_CHARS] = { 0 };
	int newQuantity = 0;
	float newPrice = 0;
	char buffer[BUFF_SIZE] = { 0 }; //does it have to be 100 magic number

	while (getchar() != '\n'); //to ensure that the buffer is cleared. when this wasn't here the program would skip taking input for the new name
	while (true)
	{
		printf("Enter the name of the new product: ");
		if (fgets(buffer, sizeof(buffer), stdin) != NULL)
		{
			buffer[strcspn(buffer, "\n")] = 0;
			if (strlen(buffer) >= MAX_CHARS)
			{
				printf("Invalid Input: Please enter a name 25 characters or less\n");
			}
			else
			{
				strcpy(newName, buffer);
				break;
			}
		}
	}

	while (true)
	{
		printf("Enter the category of the new product: ");
		if (fgets(buffer, sizeof(buffer), stdin) != NULL)
		{
			buffer[strcspn(buffer, "\n")] = 0;
			if (strlen(buffer) >= MAX_CHARS)
			{
				printf("Invalid Input: Please enter a name 25 characters or less\n");
			}
			else
			{
				strcpy(newCategory, buffer);
				break;
			}
		}
	}

	while (true)
	{
		printf("Enter the quantity: ");
		if (scanf("%d", &newQuantity) != VALID_INPUT)
		{
			printf("Invalid Input: Please enter an integer value\n");
			while (getchar() != '\n');
			continue;
		}
		else
		{
			while (getchar() != '\n');
			break;
		}
	}

	while (true)
	{
		printf("Enter the price: ");
		if (scanf("%f", &newPrice) != VALID_INPUT)
		{
			printf("Invalid Input: Please enter a float value\n");
			while (getchar() != '\n');
			continue;
		}
		else
		{
			while (getchar() != '\n');
			break;
		}
	}
	//add to struct
	newProduct->productID = 0;
	strcpy(newProduct->name, newName);
	strcpy(newProduct->category, newCategory);
	newProduct->quantity = newQuantity;
	newPrice = (float)(round(newPrice * SCND_DECIMAL) / SCND_DECIMAL);
	newProduct->price = newPrice;
}

void AddProductToList(StoreItems** productCatalog, StoreItems* newProduct, int* numProducts)
{
	StoreItems* tempCatalog = (StoreItems*)realloc(*productCatalog, (*numProducts + 1) * sizeof(StoreItems));
	if (tempCatalog == NULL)
	{
		printf("Error Reallocating Memory\n");
		return;
	}
	*productCatalog = tempCatalog;
	*numProducts += 1; //increment since we're adding a new element to the list
	newProduct->productID = *numProducts; //assign an id so it's not 0 and marked as deleted when adding back to file
	(*productCatalog)[*numProducts] = *newProduct;

}