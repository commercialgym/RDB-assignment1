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


int main(void)
{
    
	FILE* pFile = fopen("StoreCatalog.dat", "rb+");
	if (pFile == NULL)
	{
		printf("Unable to open file\n");
		return 1;
	}

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

