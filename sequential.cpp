/*
* FILE: sequential.cpp
* PROJECT: Assignment 1 - ECOMMERCE INVENTORY MANAGEMENT SYSTSEM
* PROGRAMMER: Alexia Tu
* DESCRIPTION: Reads product data from a sequential access binary file, implementing functionality to read and process data sequentially. It first reads the data from the file and prints
	* the information to the screen. Then asks the user how many items they would like to append, taking their input for new product info, and appending it to the end of the list by rewriting 
	* to the file. Then asks the user which id they would like to update, again taking new information and rewriting to the file. Then it takes the id of a product they would like to update, and
	* rewrites to the file with the new deletion. All the updates are done with the program's memory and uses dynamic memory allocation.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#pragma warning(disable:4996)
#define MAX_CHARS 26
#define VALID_INPUT 1
#define SCND_DECIMAL 100
#define BUFF_SIZE 100
#define ERROR 0
#define MAX_APPENDS 5

typedef struct {
	int productID;
	char name[MAX_CHARS];
	char category[MAX_CHARS];
	int quantity;
	float price;
}StoreItems;

long CalculateFileSize(FILE* pFile);
int NumElements(FILE* pFile);
int ReadDataFromFile(StoreItems** productCatalog);
void PrintDataFromMemory(StoreItems** productCatalog, int counter);
void AssignProductInfo(StoreItems* newProduct); //from user
void AppendProductToList(StoreItems** productCatalog, StoreItems* newProduct, int* numProduct); //adds to list in memory
bool UpdateProductInfo(StoreItems** productCatalog, int numProducts, int productID);
bool DeleteProductByID(StoreItems** productCatalog, int* numProduct, int productID);
void ResetIds(StoreItems** productCatalog, int* numProducts);
void WriteToFile(StoreItems* productCatalog, int numProducts);
void FreeProductCatalog(StoreItems** productCatalog);

int main(void)
{
	StoreItems* productCatalog = NULL; //used for product data manipulation in memory

	/* -------- READING DATA FROM FILE ------*/

	//read the data from the file sequentially
	int numProducts = ReadDataFromFile(&productCatalog);
	//and print the data that is now held in memory
	PrintDataFromMemory(&productCatalog, numProducts);

	/* --------- APPENDING PRODUCTS AND WRITING TO FILE -------------- */
	//update the data and write the entire set back to the file in sequence
	int numToAdd = 0;
	while (true)
	{
		printf("\nHow Many New Products Do You Wish To Append (1-%d): ", MAX_APPENDS);
		if (scanf("%d", &numToAdd) != VALID_INPUT)
		{
			printf("Invalid input.\n");
			while (getchar() != '\n');
			continue;
		}
		if (numToAdd >= 1 && numToAdd <= MAX_APPENDS)
		{
			while (getchar() != '\n');
			break;
		}
	}
	for (int i = 0; i < numToAdd; i++)
	{
		StoreItems newProduct = { 0, "Deleted", "Deleted", 0, 0 };
		printf("\nAppending New Product #%d to The Catalog:\n", i + 1);
		AssignProductInfo(&newProduct);
		AppendProductToList(&productCatalog, &newProduct, &numProducts);
	}
	//write to file
	WriteToFile(productCatalog, numProducts);
	FreeProductCatalog(&productCatalog);

	/* ----------- UPDATE A PRODUCT AND WRITE TO FILE ------------- */

	numProducts = ReadDataFromFile(&productCatalog); //data read from here is used in next functionality
	//update a product
	int productID = 0;
	printf("\nThere are Currently %d Product IDs\n", numProducts);
	while (true)
	{
		printf("Enter the ID of a Product to Update: ");
		if (scanf("%d", &productID) != VALID_INPUT)
		{
			printf("Invalid input.\n");
			while (getchar() != '\n');
			continue;
		}
		while (getchar() != '\n');
		if (UpdateProductInfo(&productCatalog, numProducts, productID))
		{
			break; //if the user entered a successful ID then continue on with the program.
		}
	}
	//write to file
	WriteToFile(productCatalog, numProducts);
	FreeProductCatalog(&productCatalog);

	/* ------------- DELETE A PRODUCT AND WRITE BACK TO FILE -------------- */

	ReadDataFromFile(&productCatalog);
	printf("\nThere are Currently %d Product IDs\n", numProducts);
	while (true)
	{
		printf("Enter the ID of a Product to Delete: ");
		if (scanf("%d", &productID) != VALID_INPUT)
		{
			printf("Invalid input.\n");
			while (getchar() != '\n');
			continue;
		}
		while (getchar() != '\n');
		if (DeleteProductByID(&productCatalog, &numProducts, productID))
		{
			break;
		}
	}
	//now rewrite the entire file
	WriteToFile(productCatalog, numProducts);

	FreeProductCatalog(&productCatalog);

	return 0;
}

//NAME: CalculateFileSize
//PARAMETER: FILE* pFile
//DESCRIPTION: calculates the total bytes that are in the file by setting the file pointer to the very end and using ftell to return how many bytes in to the file the pointer is located.
// it's used by te NumElements function to determine how many elements there are. Also, the file pointer is stored in a variable at the beginning of the function and is restored at the end
// to ensure that the main menu functions as expected.
//RETURNS: the total number of bytes in the file
long CalculateFileSize(FILE* pFile)
{
	long currentPos = ftell(pFile); //capture the current fp position
	fseek(pFile, 0, SEEK_END); //reminder that this means the fp is placed at the end, should i move it to the beginning?
	long totalSize = ftell(pFile);
	fseek(pFile, currentPos, SEEK_SET); //return the fp position to the previous
	return totalSize;
}

//NAME: NumElements
//PARAMETER: FILE* pFile
//DESCRIPTION: uses the calculatefilesize function to get the total number of bytes and then divides that value by the number of bytes that an element takes to find the total number of elements
//RETURNS: total number of elements
int NumElements(FILE* pFile)
{
	return CalculateFileSize(pFile) / sizeof(StoreItems);
}

//NAME: ReadDataFromFile
//PARAMETER: StoreItems** productCatalog
//DESCRIPTION: Opens the file, calculates the number of elements and allocates a block of memory for a pointer array, then reads the data to the pointer array sequentially, keeping track of how many elements are read.
// It then closes the file.
//RETURNS: and int number of elements read
int ReadDataFromFile(StoreItems** productCatalog)
{
	FILE* pFile = fopen("StoreCatalog.dat", "rb+");
	if (pFile == NULL)
	{
		printf("Unable to open file\n");
		return 1;
	}
	int count = NumElements(pFile);
	*productCatalog = (StoreItems*)malloc(count * sizeof(StoreItems));
	if (*productCatalog == NULL)
	{
		printf("Error Allocating Memory\n");
		return -1;
	}

	int index = 0;
	while (index < count)
	{
		if (fread(&(*productCatalog)[index], sizeof(StoreItems), 1, pFile) != VALID_INPUT)
		{
			printf("Error Reading From File\n");
			return false;
		}
		index++;
	}

	if (fclose(pFile) == EOF)
	{
		printf("Error closing file\n\n");
		clearerr(pFile);
	}

	return count;
}

//NAME: PrintDataFromMemory
//PARAMETER: StoreItems** productCatalog, int counter
//DESCRIPTION: indexes through the block of memory containing the data read from the file, printing each product's information to the console. it only prints the number of elements that is known 
// to ensure that no deleted product is printed, this condition is not however met with the current test harness.
//RETURNS: void
void PrintDataFromMemory(StoreItems** productCatalog, int counter)
{
	printf("\nCatalog Data From File:");
	for (int index = 0; index < counter; index++)
	{
		printf("\nProduct ID: %d\n", (*productCatalog)[index].productID);
		printf("Name: %s\n", (*productCatalog)[index].name);
		printf("Category: %s\n", (*productCatalog)[index].category);
		printf("Quantity: %d\n", (*productCatalog)[index].quantity);
		printf("Price: %.2f\n", (*productCatalog)[index].price);
	}
}

//NAME: AssignProductInfo
//PARAMETER: StoreItems* newProduct
//DESCRIPTION: handles taking user input to assign values to the fields of the struct passed to this function. it can be used for either updating a product or creating
// a brand new one
//RETURNS: void
void AssignProductInfo(StoreItems* newProduct)
{
	char newName[MAX_CHARS] = { 0 };
	char newCategory[MAX_CHARS] = { 0 };
	int newQuantity = 0;
	float newPrice = 0;
	char buffer[BUFF_SIZE] = { 0 };

	//while (getchar() != '\n'); //to ensure that the buffer is cleared. when this wasn't here the program would skip taking input for the new name
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

//NAME: AppendProductToList
//PARAMETER: StoreItems** productCatalog, StoreItems* newProduct, int* numProducts
//DESCRIPTION: reallocates memory for the pointer array to ensure there is another block of memory available for the incoming product. that block of memory is then assigned the value of the new product
// the number of products counter is also incremented for consistency
//RETURNS: void
void AppendProductToList(StoreItems** productCatalog, StoreItems* newProduct, int* numProducts)
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
	(*productCatalog)[*numProducts - 1] = *newProduct;
}

//NAME: UpdateProductInfo
//PARAMETER: StoreItems** productCatalog, int numProducts, int productID
//DESCRIPTION: ensures that the id passed is within valid range and then calls the assignproductinfo function to gather the new info. then assigns the correct input id back to the new struct.
// the updated struct is then assigned to the index of the old product memory
//RETURNS: true on success false on failure
bool UpdateProductInfo(StoreItems** productCatalog, int numProducts, int productID)
{
	StoreItems replacementItem = { 0, "Deleted", "Deleted", 0, 0 };

	if (productID < 1 || productID > numProducts)
	{
		printf("Invalid ID: Please Enter A Number Between 1 and %d\n", numProducts);
		return false;
	}

	AssignProductInfo(&replacementItem);
	replacementItem.productID = productID;

	(*productCatalog)[productID - 1] = replacementItem;

	return true;
}

//NAME: DeleteProductByID
//PARAMETER: StoreItems** productCatalog, int* numProducts, int productID
//DESCRIPTION: ensures that the id passed is within valid range and assigns the place in memory with deletion values. it then calls the resetIDs function to shuffle the products down to
// ensure that when the list is written back to the file, the product id's maintain integrity
//RETURNS: true on success false on failure
bool DeleteProductByID(StoreItems** productCatalog, int* numProducts, int productID)
{
	if (productID < 1 || productID > *numProducts)
	{
		printf("Invalid ID: Please Enter A Number Between 1 and %d\n", *numProducts);
		return false;
	}
	StoreItems deleteItem = { 0, "Deleted", "Deleted", 0, 0 };
	//go to index of product id
	(*productCatalog)[productID - 1] = deleteItem;
	ResetIds(productCatalog, numProducts);
	return true;
}

//NAME: ResetIds
//PARAMETER: StoreItems** productCatalog, int* numProducts
//DESCRIPTION: takes the pointer array and iterates through the list to find any deleted items. if an item has not been deleted, it ensures that the shiftindex is alligned with the current index,
// if it's not aligned then the gap is filled by "shifting" the products down by one. then for any values past the valid number of shift indexes, their id's are assigned 0 to mark as deleted. then
// the id's of the previously shifted id's are assigned their new index + 1. then the number of products is updated so that when writing back to the file, the number of fwrites are only that of the number
// of valid products. therefore the space that is marked as deleted does not get written even though they technically exist.
//RETURNS: void
void ResetIds(StoreItems** productCatalog, int* numProducts)
{
	int shiftIndex = 0; 
	for (int i = 0; i < *numProducts; i++)
	{
		//if product id is not marked as deleted
		if ((*productCatalog)[i].productID != 0) {
			//fill in the gap by shifting valid products down
			if (shiftIndex != i) {
				(*productCatalog)[shiftIndex] = (*productCatalog)[i];
			}
			shiftIndex++;
		}
	}
	//mark the last ID as 0
	for (int i = shiftIndex; i < *numProducts; i++) {
		(*productCatalog)[i].productID = 0; //means deleted
	}
	//reassign ids
	for (int i = 0; i < shiftIndex; i++) {
		(*productCatalog)[i].productID = i + 1; //set id to index + 1
	}
	*numProducts = shiftIndex; //update counter for consistency
}

//NAME: WriteToFile
//PARAMETER: StoreItems* productCatalog, int numProducts
//DESCRIPTION: opens a file and truncates it to zero, then writes the product catalog to the file sequentially. it only writes the number of times that there are numProducts,
// this is because there may be products that were deleted that are now counted in the total numProducts, and they should not be written to the file.
//RETURNS: void 
void WriteToFile(StoreItems* productCatalog, int numProducts)
{
	FILE* pFile = fopen("StoreCatalog.dat", "wb+"); //truncate the file to overwrite with "new data"
	if (pFile == NULL)
	{
		printf("Unable to open file\n");
		return;
	}
	for (int i = 0; i < numProducts; i++)
	{
		if (fwrite(&productCatalog[i], sizeof(StoreItems), 1, pFile) != VALID_INPUT)
		{
			printf("Error Writing to File at Index %d\n", i);
			return;
		}
	}
	if (fclose(pFile) == EOF)
	{
		printf("Error closing file\n\n");
		clearerr(pFile);
	}
}

//NAME: FreeProductCatalog
//PARAMETER: StoreItems** productCatalog
//DESCRIPTION: frees all dynamically allocated space
//RETURNS: void
void FreeProductCatalog(StoreItems** productCatalog)
{
	if (productCatalog != NULL && *productCatalog != NULL)
	{
		free(*productCatalog);
		*productCatalog = NULL;
	}
}