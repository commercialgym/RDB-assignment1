/*
* FILE: random.cpp
* PROJECT: Assignment 1 - ECOMMERCE INVENTORY MANAGEMENT SYSTSEM
* PROGRAMMER: Alexia Tu
* DESCRIPTION: This program stores product data in a random access binary file. It implements the functionality to read, update, create (append), and delete specific
	* product data by seeking to the correct position in the file based off of a given product id / index. It uses a menu based system to allow a user to select what
	* they would like to do. This includes: Reading a products info from it's id, updating a product, deleting a product, reusing space from a deleted product's index, appending a new
	* product to the end of the file, reading a products info from the current positon in the file, indexing forwards and indexing backwards. There a 12 addtional functions
	* that are used to conduct these functionalities.
*/

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
#define ERROR -1
#define SUCCESS 0
#define EXIT 9
#define SCND_DECIMAL 100
#define BUFF_SIZE 100

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

//ADD ERROR CHECKING FOR FREAD AND FWRITE AND FSEEK AND FTELL

int main(void)
{

	FILE* pFile = fopen("StoreCatalog.dat", "rb+"); 
	if (pFile == NULL)
	{
		printf("Unable to open file\n");
		return 1;
	}

	int productID = 0; //user input for product ID to manipulate
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


	if (fclose(pFile) == EOF) {
		printf("Error closing file\n\n");
		clearerr(pFile);
	}

	return 0;
}

//NAME: indexFilePosition
//PARAMETER: FILE* pFile, int productID
//DESCRIPTION: checks to see if the product id is within the allowed range by using the NumElements function. Then it multiples the number of indexes in the file
// by the size of each element to return how many bytes into the file that product is located.
//RETURNS: ERROR if the product ID is not in range, OR the number of bytes into the file the product is found at if the entered id is within range
long indexFilePosition(FILE* pFile, int productID)
{
	if (productID <= 0 || productID > NumElements(pFile))
	{
		printf("Product ID Not Found: Please Enter within Range 1 - %d\n\n", NumElements(pFile)); //should i put errors in main function?
		return ERROR;
	}
	return (productID - 1) * sizeof(StoreItems);
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

//NAME: ReadDataFromID
//PARAMETER: FILE* pFile, int productID, StoreItems* readItem(readItem is a blank struct to store the info that is read)
//DESCRIPTION: takes a product id and finds it's file position with the indexFilePositionFunction. then it reads the data at that position into the blank struct that is passed to the function
//RETURNS: true upon success and false upon failrue
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

//NAME: ReadDataFromCurrentIndex
//PARAMETER: FILE* pFile, StoreItems* readItem
//DESCRIPTION: uses ftell to get the current file position then reads starting from the front edge of the block of memory for the products informations
//if the current file position is at the beginning, the index will be zero, so it needs to be incremented by one to correlate with id of the product at the start of that memory block.
// It increments the index to be associated with the expected id, then sends the the correct index id to the readdatafromid function.
//RETURNS: true upon success and false upon failure
bool ReadDataFromCurrentIndex(FILE* pFile, StoreItems* readItem)
{
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
	if (indexID > NumElements(pFile) || indexID < 1) 
	{
		printf("End of File: no product to read\n");
		return false;
	}
	ReadDataFromID(pFile, indexID, readItem);
	return true;
}

//NAME: AddNewProduct
//PARAMETER: FILE* pFile
//DESCRIPTION: appends to the end of file by calling the assignproductinfo function to gather the new information, then assigning it's id with the number of elements there are plus 1. then appentds by using fseek and fwrite
//RETURNS: true on success false on failure
bool AddNewProduct(FILE* pFile)
{
	StoreItems newProduct = { 0, "new", "new", 0, 0 };
	AssignProductInfo(&newProduct);
	//need to assign appropriate product ID
	int newID = NumElements(pFile);
	newID += 1; //index by one
	newProduct.productID = newID;
	//now write to file
	if (fseek(pFile, 0, SEEK_END) == ERROR)
	{
		return false;
	}
	if (fwrite(&newProduct, sizeof(StoreItems), 1, pFile) != VALID_INPUT)
	{
		return false;
	}
	return true;
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

//NAME: PrintProductInfo
//PARAMETER: StoreItems* product
//DESCRIPTION: prints the product information of the struct passed to it. if the id is zero that means that the product has been deleted. this condition may be met if a user deletes
// a product and then indexes to the positon and selects the option to read the product of the current index.
//RETURNS: void
void PrintProductInfo(StoreItems* product)
{
	if (product->productID == 0)
	{
		printf("Error Reading: The product at this index has been deleted\n");
		return;
	}
	printf("\nProduct ID: %d\n", product->productID);
	printf("Name: %s\n", product->name);
	printf("Category: %s\n", product->category);
	printf("Quantity: %d\n", product->quantity);
	printf("Price: %.2f\n", product->price);
}

//NAME: DeleteProductByID
//PARAMETER: FILE* pFile, int productID
//DESCRIPTION: uses indexfileposition to ensure that the id passed to the function is valid. then uses the index returned from that function to seek to the product index, reads to ensure that
// the product has not already been deleted, then uses a temp struct to assign deletion values and write the new deleted valued struct to the file, marking it as deleted.
//RETURNS: true upon success and false upon failure
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
		if (fseek(pFile, productIndex, SEEK_SET) == ERROR)
		{
			return false;
		}
		if (fwrite(&deleteItem, sizeof(StoreItems), 1, pFile) != VALID_INPUT)
		{
			return false;
		}
	}
	return true;
}

//NAME: UpdateProductInfo
//PARAMETER: FILE* pFile, int productID, int updateOrReuse
//DESCRIPTION: checks the value, of the id passed to the function, in the file to ensure that if a user wants to update a product, the index that they've requested has not been deleted. OR if the
// user wants to reuse deleted space, ensures that the index of the id passed has actually been deleted. if either of these conditions have been met then the function continues and takes new input
// by calling the assignproductinfo function and assigning the correct id, then writing to the file
//RETURNS: true on success false on failure
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
	if (fseek(pFile, productIndex, SEEK_SET) == ERROR)
	{
		return false;
	}
	if (fwrite(&replacementItem, sizeof(StoreItems), 1, pFile) != VALID_INPUT)
	{
		return false;
	}
	return true;
}

//NAME: IndexForwards
//PARAMETER: FILE* pFile
//DESCRIPTION: moves the file pointer forwards by the number of bytes one element takes up
//RETURNS: void
void IndexForwards(FILE* pFile)
{
	if (ftell(pFile) + sizeof(StoreItems) >= CalculateFileSize(pFile)) 
	{
		printf("Error Indexing Forward: End of File\n");
		return;
	}
	fseek(pFile, sizeof(StoreItems), SEEK_CUR);
}

//NAME: IndexBackwards
//PARAMETER: FILE* pFile
//DESCRIPTION: moves the file pointer backwards by the number of bytes one element takes up
//RETURNS: void
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