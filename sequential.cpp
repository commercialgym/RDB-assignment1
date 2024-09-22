// sequential.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#pragma warning(disable:4996)
#define MAX_CHARS 26
#define VALID_INPUT 1

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

	ReadDataFromFile(pFile, &productCatalog, numProducts);

	//read the data from the file sequentially

	if (fclose(pFile) == EOF) {
		printf("Error closing file\n\n");
		clearerr(pFile);
	}

		//and print the data that is now held in memory

	//update the data and write the entire set back to the file in sequence
		//maybe take user input?

		//add a new product

		//update a product

		//delete a product

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
//relies on other file to return proper number of bytes, may combine these two later?
int NumElements(FILE* pFile)
{
	return CalculateFileSize(pFile) / sizeof(StoreItems);
}

bool ReadDataFromFile(FILE* pFile, StoreItems **productCatalog, int count)
{
	int index = 0;
	while (index < count)
	{
		if (fread(productCatalog[index], sizeof(StoreItems), 1, pFile) != VALID_INPUT)
		{
			printf("Error Reading From File\n");
			return false;
		}
		index++;
	}
	return true;
}