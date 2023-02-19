#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "array.h"
#include "random.h"
#include "algorithm.h"

#define ARRAY_MAX_SIZE 1000000

//colors
#define DEFAULT 0
#define YELLOW 1

void throwErrorAndClearInputBuffer(char* errorMsg) 
{
	printf(errorMsg);
	rewind(stdin);
}

void setTextColor(int colorID)
{
	switch (colorID)
	{
	case YELLOW:
		printf("\033[1;33m");
		break;
	default:
		printf("\033[0m");
		break;
	}
}

void generateArrayElementsByRandom(int* array, unsigned int arraySize) 
{
	for (int i = 0; i < arraySize; i++)
		array[i] = randomRange(-1000, 1000);
}

void readArrayElementsFromInput (int* array, unsigned int arraySize) 
{
	char lastChar;

	for (int i = 0; i < arraySize; i++)
		while (scanf_s("%d", &array[i]) < 1 || ((lastChar = getchar()) != '\n' && lastChar != ' '))
			throwErrorAndClearInputBuffer("Incorrect input, please try enter element again: ");
}

void printArrayWithHighlightedEvenElements(int* array, unsigned int arraySize, int colorID) 
{
	char* buffer = NULL;

	if (arraySize >= OUTPUT_OPTIMIZATION_ARRAY_SIZE)
	{
		buffer = calloc(arraySize, sizeof(char) * 5);
		setvbuf(stdout, buffer, _IOFBF, arraySize * sizeof(char) * 5);
	}

	for (int i = 0; i < arraySize; i++)
	{
		if ((i + 1) % 2 == 0)
			setTextColor(colorID);

		printf("%4d ", array[i]);

		setTextColor(DEFAULT);
	}

	if (arraySize >= OUTPUT_OPTIMIZATION_ARRAY_SIZE)
	{
		setvbuf(stdout, buffer, _IONBF, arraySize * sizeof(char) * 5);
		free(buffer);
	}

	printf("\n");
}

void userInputArray(int** arrayPtr, unsigned int* arraySize) 
{
	printf("Enter array size: ");

	while (scanf_s("%u", arraySize) < 1 || getchar() != '\n' || *arraySize < 1 || *arraySize > ARRAY_MAX_SIZE)
	{
		if (*arraySize < 1 || *arraySize > ARRAY_MAX_SIZE)
			throwErrorAndClearInputBuffer("Size out of range, please try enter size again (1 - 1000000): ");
		else
			throwErrorAndClearInputBuffer("Incorrect input, please try enter size again: ");
	}

	*arrayPtr = initArray(*arraySize);

	char choice;

	printf("Do you want to enter matrix elements? (Otherwise elements will genereted by random in range from -10^3 to 10^3)\n(Y/N): ");

	while (scanf_s("%c", &choice, 1) < 1 || getchar() != '\n' || (choice != 'Y' && choice != 'N'))
		throwErrorAndClearInputBuffer("Incorrect input, enter answer in format (Y/N): ");

	if (choice == 'Y')
	{
		printf("Enter array elements: ");
		readArrayElementsFromInput(*arrayPtr, *arraySize);
	}

	else
	{
		generateArrayElementsByRandom(*arrayPtr, *arraySize);
	}
}

int main()
{
	unsigned int arraySize;
	int* array;

	setRandomSeed(time(0));

	userInputArray(&array, &arraySize);
	printf("\nCurrent state of array:\n");
	printArray(array, arraySize);

	int* copiedArray = copyArray(array, arraySize);

	//
	clock_t startTimePoint = clock();

	quickSort(array, 1, arraySize % 2 == 0 ? arraySize - 1 : arraySize - 2, 2);

	clock_t endTimePoint = clock();

	printf("\nState of array after quick sorting even elements:\n");
	printArrayWithHighlightedEvenElements(array, arraySize, YELLOW);

	printf("\nTime spend on sorting - quick sort algorithm: %ld ms\n", endTimePoint - startTimePoint);

	startTimePoint = clock();

	bubleSort(copiedArray, arraySize % 2 == 0 ? arraySize : arraySize - 1, 2);

	endTimePoint = clock();

	printf("Time spend on sorting - buble sort algorithm: %ld ms\n", endTimePoint - startTimePoint);
	//

	quickSort(array, 0, arraySize, -1);
	
	free(copiedArray);

	copiedArray = copyArray(array, arraySize);

	printf("\nTimings in case when array initially sorted descending:\n");

	//
	startTimePoint = clock();

	quickSort(array, 1, arraySize % 2 == 0 ? arraySize - 1 : arraySize - 2, 2);

	endTimePoint = clock();

	printf("\nTime spend on sorting - quick sort algorithm: %ld ms\n", endTimePoint - startTimePoint);

	startTimePoint = clock();

	bubleSort(copiedArray, arraySize % 2 == 0 ? arraySize : arraySize - 1, 2);

	endTimePoint = clock();

	printf("Time spend on sorting - buble sort algorithm: %ld ms\n", endTimePoint - startTimePoint);
	//

	free(array);
	free(copiedArray);

	return 0;
}