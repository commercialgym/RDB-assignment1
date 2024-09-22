// assignment1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#pragma warning(disable:4996)
#define MAX_CHARS 26
#define VALID_INPUT 1
#define NEW_PRODUCT 0
#define UPDATE 1
#define REUSE 2
#define ITEM_SIZE (sizeof(int) + NAME_SIZE + CATEGORY_SIZE + sizeof(int) + sizeof(float)) //to be used for accessing the index of a file
#define ERROR -1
#define SUCCESS 0
#define EXIT 9
#define SCND_DECIMAL 100

typedef struct {
	int productID;
	char name[MAX_CHARS];
	char category[MAX_CHARS];
	int quantity;
	float price;
}StoreItems;

long indexFilePosition(FILE* pFile, int productID);
long CalculateFileSize(FILE* pFile);
int NumElements(FILE* pFile);
bool ReadDataFromID(FILE* pFile, int productID, StoreItems* readItem);
bool ReadDataFromCurrentIndex(FILE* pFile, StoreItems* readItem);
bool AddNewProduct(FILE* pFile);
void AssignProductInfo(StoreItems* newProduct); //from user
void PrintProductInfo(StoreItems* product);
bool DeleteProductByID(FILE* pFile, int productID);
bool UpdateProductInfo(FILE* pFile, int productID, int updateOrReuse);
void IndexForwards(FILE* pFile);
void IndexBackwards(FILE* pFile);

int main(void)
{

	FILE* pFile = fopen("StoreCatalog.dat", "rb+"); //change to rb+ for when actually coding the program
	if (pFile == NULL)
	{
		printf("Unable to open file\n");
		return 1;
	}

	int productID = 0; //user input for product ID to manipulate
	long int filePos = 0; //TESTING
	int indexID = 0; //TESTING
	printf("Welcome to My Random Access Program! Please Select What You'd Like to Do: \n");
	fseek(pFile, 0, SEEK_SET);
	while (true)
	{
		int choice = 0; //user input for menu option
		StoreItems productInfo = { 0, "Deleted", "Deleted", 0, 0 };
		printf("1. Read a product's information\n");
		printf("2. Update a product's information\n");
		printf("3. Delete a product from the file\n");
		printf("4. Reuse the space from a deleted product\n");
		printf("5. Append a new product to the end of the file\n");
		printf("6. Read the info of the current file index\n");
		printf("7. Index forwards in the file\n");
		printf("8. Index backwards in the file\n");
		printf("9. Exit\n");
		printf("There are currently %d indexes of space in use in the file\n", NumElements(pFile));
		printf("Enter your choice: ");
		if (scanf("%d", &choice) != VALID_INPUT)
		{
			printf("Invalid Input: Please enter a valid menu option\n");
			while (getchar() != '\n');
			continue;
		}

		switch (choice)
		{
		case 1: 
			printf("Enter the ID of the product you wish to read: ");
			if (scanf("%d", &productID) != VALID_INPUT)
			{
				printf("Invalid input.\n");
				while (getchar() != '\n'); 
				continue;
			}
			if (ReadDataFromID(pFile, productID, &productInfo))
			{
				PrintProductInfo(&productInfo);
			}
			break;
		case 2:
			printf("Enter the ID of the product to update: ");
			if (scanf("%d", &productID) != VALID_INPUT)
			{
				printf("Invalid input.\n");
				while (getchar() != '\n');
				continue;
			}
			UpdateProductInfo(pFile, productID, UPDATE);
			break;
		case 3:
			printf("Enter the ID of the product to delete: ");
			if (scanf("%d", &productID) != VALID_INPUT)
			{
				printf("Invalid input.\n");
				while (getchar() != '\n');
				continue;
			}
			DeleteProductByID(pFile, productID);
			break;
		case 4: 
			printf("Enter the index of the space to reuse: ");
			if (scanf("%d", &productID) != VALID_INPUT)
			{
				printf("Invalid input.\n");
				while (getchar() != '\n');
				continue;
			}
			UpdateProductInfo(pFile, productID, REUSE);
			break;
		case 5:
			AddNewProduct(pFile);
			break;
		case 6:
			if (ReadDataFromCurrentIndex(pFile, &productInfo)) //every time you read from the current index it will increment the fp forwards by one
			{
				PrintProductInfo(&productInfo);
			}
			break;
		case 7:
			IndexForwards(pFile);
			break;
		case 8: 
			IndexBackwards(pFile);
			break;
		case 9:
			printf("Goodbye!\n");
			break;
		default:
			printf("Invalid Entry Please Try Again\n");
		}
		if (choice == EXIT)
		{
			break;
		}
		printf("\n");
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

	//access a product by it's index and change the info

	//read the product info of the current index (use ftell and call readdata function)


	//might write an arbitrary amount of data to the file initially to use later for read, update, delete

	//TESTING
	//StoreItems newItem;
	//GetNewProductInfo(&newItem);
	//PrintProductInfo(&newItem);

	//ReadDataFromID(pFile, 1, &newItem);
	//PrintProductInfo(&newItem);


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
long indexFilePosition(FILE* pFile, int productID)
{
	if (productID <= 0 || productID > NumElements(pFile))
	{
		printf("Product ID Not Found: Please Enter within Range 1 - %d\n\n", NumElements(pFile)); //should i put errors in main function?
		return ERROR;
	}
	return (productID - 1) * sizeof(StoreItems);
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

//NAME:
//PARAMETER: readItem is a blank struct to store the info that is read
//DESCRIPTION:
//RETURNS: 
bool ReadDataFromID(FILE* pFile, int productID, StoreItems* readItem)
{
	int fileIndex = indexFilePosition(pFile, productID);
	if (fseek(pFile, fileIndex, SEEK_SET) == ERROR)
	{
		return false;
	}
	//StoreItems readItem; 
	if (fread(readItem, sizeof(StoreItems), 1, pFile) == ERROR)
	{
		printf("Error Reading From Given Index\n");
		return false;
	}
	return true;
}

//if the current file position is at the beginning, the index will be zero, so it needs to be incremented by one
//to correlate with id of the product at the start of that memory block.
bool ReadDataFromCurrentIndex(FILE* pFile, StoreItems* readItem) //reads from the front edge of the block of memory
{
	fseek(pFile, 0, SEEK_CUR);
	long int filePos = ftell(pFile);
	if (filePos == ERROR)
	{
		return false;
	}
	int indexID = filePos / sizeof(StoreItems);

	if (filePos % sizeof(StoreItems) != 0)
	{
		printf("FP misaligned\n");
		return false;
	}

	indexID += 1; //ensure that the id is at the starting block of memory
	if (indexID > NumElements(pFile) || indexID < 1) //im not sure if this condition would ever be met
	{
		printf("End of File: no product to read\n");
		return false;
	}
	ReadDataFromID(pFile, indexID, readItem);
	return true;
}

//NAME:
//PARAMETER: &newProduct (pass as a reference?)
//DESCRIPTION: appends to the end of file
//RETURNS: 
bool AddNewProduct(FILE* pFile)
{
	StoreItems newProduct = { 0, "new", "new", 0, 0 };
	AssignProductInfo(&newProduct);
	//need to assign appropriate product ID
	int newID = NumElements(pFile);
	newID += 1; //index by one
	newProduct.productID = newID;
	//now write to file
	fseek(pFile, 0, SEEK_END);
	fwrite(&newProduct, sizeof(StoreItems), 1, pFile);
	return true;
}

//NAME:
//PARAMETER: NEEDS TO BE FIXED TO NOT BE IN AN IFINITE WHILE LOOP USING FGETS AND SSCANF
//DESCRIPTION:
//RETURNS: StoreItems by value
void AssignProductInfo(StoreItems* newProduct)
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
			continue;
		}
		else
		{
			break;
		}
	}
	//add to struct
	newProduct->productID = 0;
	strcpy(newProduct->name, newName);
	strcpy(newProduct->category, newCategory);
	newProduct->quantity = newQuantity;
	newPrice = (float)(round(newPrice * SCND_DECIMAL) / SCND_DECIMAL); //magic number
	newProduct->price = newPrice;
	//printf("%d, %s, %s, %d, %f\n", newProduct.productID, newProduct.name, newProduct.category, newProduct.quantity, newProduct.price);
	//return newProduct;
}

//NAME:
//PARAMETER:
//DESCRIPTION:
//RETURNS: 
void PrintProductInfo(StoreItems* product)
{
	if (product->productID == 0)
	{
		printf("Error Reading: The product at this index has been deleted\n");
	}
	printf("%d, %s, %s, %d, %.2f\n", product->productID, product->name, product->category, product->quantity, product->price);
}

bool DeleteProductByID(FILE* pFile, int productID)
{
	int productIndex = indexFilePosition(pFile, productID);
	if (productIndex == ERROR)
	{
		return false;
	}
	StoreItems deleteItem = { 0, "Deleted", "Deleted", 0, 0 };
	//go to index of product id
	fseek(pFile, productIndex, SEEK_SET);
	ReadDataFromID(pFile, productID, &deleteItem);
	if (deleteItem.productID == 0)
	{
		printf("Error Deleting: Product has already been deleted from this index\n");
		return false;
	}
	else
	{
		deleteItem.productID = 0;
		strcpy(deleteItem.name, "Deleted");
		strcpy(deleteItem.category, "Deleted");
		deleteItem.quantity = 0;
		deleteItem.price = 0;
		//go to index of product id
		fseek(pFile, productIndex, SEEK_SET);
		fwrite(&deleteItem, sizeof(StoreItems), 1, pFile);
		//did that overwrite it properly?
	}
	return true;
}

bool UpdateProductInfo(FILE* pFile, int productID, int updateOrReuse)
{
	int productIndex = indexFilePosition(pFile, productID);
	StoreItems replacementItem = { 0, "Deleted", "Deleted", 0, 0 };
	if (ReadDataFromID(pFile, productID, &replacementItem) == false) //assigns proper id to the the replacementItem field
	{
		return false;
	}
	if (replacementItem.productID == 0 && updateOrReuse == UPDATE) //means that this space is free (was deleted previously) and is being reused
	{
		printf("Error Updating: The product at this index has been deleted\n");
		return false;
	}
	else if (replacementItem.productID != 0 && updateOrReuse == REUSE)
	{
		printf("Error Reusing Space: The product at this index has NOT been deleted\n");
		return false;
	}
	else if (replacementItem.productID == 0 && updateOrReuse == REUSE)
	{
		replacementItem.productID = productID;
	}

	AssignProductInfo(&replacementItem);
	replacementItem.productID = productID;
	fseek(pFile, productIndex, SEEK_SET);
	fwrite(&replacementItem, sizeof(StoreItems), 1, pFile);
	//does there need to be error checking for reading and writing to a file?
	return true;
}

void IndexForwards(FILE* pFile)
{
	if (ftell(pFile) + sizeof(StoreItems) >= CalculateFileSize(pFile)) 
	{
		printf("Error Indexing Forward: End of File\n");
		return;
	}
	fseek(pFile, sizeof(StoreItems), SEEK_CUR);
}


void IndexBackwards(FILE* pFile)
{
	long currentPos = ftell(pFile);
	if ((int)(currentPos - sizeof(StoreItems)) < 0)
	{
		printf("Error Indexing Backwards: Beginning of File\n");
		return;
	}

	long int offset = sizeof(StoreItems);
	fseek(pFile, -offset, SEEK_CUR);
}