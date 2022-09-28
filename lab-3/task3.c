#include <stdio.h>

#define MAX_SIZE_OF_ARRAY 100
#define INPUT_ARRAY_TYPE char
#define GROWING_ARRAY 0
#define DESCENDING_ARRAY 1

long long randomState;
const long long randomMax = 4294967296;

unsigned int randomByte()
{
	randomState = 214013 * randomState + 2531011;
	randomState ^= randomState >> 15;
	return (unsigned int)(randomState % randomMax);
}

float randomRange(float min, float max)
{
	return (randomByte() % ((int)(max * 1000) - (int)(min * 1000)) + (int)(min * 1000)) / 1000.f;
}

void throwErrorAndClearInputBuffer(const char* errorMsg)
{
	printf(errorMsg);
	rewind(stdin);
}

void userInputArray(float* array, unsigned int* arraySizePtr, INPUT_ARRAY_TYPE arrayType)
{
	printf("Enter size of array in range: 1 - 100: ");

	while (*arraySizePtr < 1 || *arraySizePtr > MAX_SIZE_OF_ARRAY)
	{
		while (scanf_s("%u", arraySizePtr) < 1)
			throwErrorAndClearInputBuffer("Incorrect input, please try enter data again: ");

		if (*arraySizePtr < 1 || *arraySizePtr > MAX_SIZE_OF_ARRAY)
			throwErrorAndClearInputBuffer("Size out of rang (1 - 100), please enter correct data: ");
	}

	char choice = '\0';

	printf("\nDo you want to enter array elements? (Otherwise elements will genereted by random in range from -10^2 to 10^4)\n(Y/N): ");

	rewind(stdin);

	while (scanf_s("%c", &choice, 1) < 1 || (choice != 'N' && choice != 'Y'))
		throwErrorAndClearInputBuffer("Incorrect input, enter answer in format (Y/N): ");

	if (choice == 'Y')
		for (int i = 0; i < *arraySizePtr; i++)
			while (scanf_s("%f", &array[i]) < 1 || (arrayType == GROWING_ARRAY ? (i > 0 ? array[i] < array[i - 1] : 0) : (i > 0 ? array[i] > array[i - 1] : 0)))
			{
				throwErrorAndClearInputBuffer("Incorrect input, input should be a floating point number, and current number shoulde be %s then pervious\n", 
					arrayType == GROWING_ARRAY ? "bigger" : "less");

				printf("Current array state: ");
				for (int j = 0; j < i; j++)
				{
					printf(" %.3f", array[j]);

					printf(j + 1 == i ? ";\n" : ",");
				}

				printf("Enter %i %s again: ", *arraySizePtr - i, *arraySizePtr - i == 1 ? "number" : "numbers");
			}
	else
		for (int i = 0; i < *arraySizePtr; i++)
			if (arrayType == GROWING_ARRAY)
				if (i != 0)
					array[i] = array[i - 1] + randomRange(0, 100);
				else
					array[i] = randomRange(0, 1) > 0.5f ? randomRange(0, 1000) : -randomRange(0, 1000);
			else
				if (i != 0)
					array[i] = array[i - 1] - randomRange(0, 100);
				else
					array[i] = randomRange(0, 1) > 0.5f ? randomRange(0, 1000) : -randomRange(0, 1000);

	printf("\nCurrent array state:\n");

	for (int i = 0; i < *arraySizePtr; i++)
		printf("[%i]\t%.3f\n", i, array[i]);
}

void moveElementsInArray(float* array, unsigned int arraySize, unsigned int startIndex) 
{
	for (int i = arraySize - 1; i >= startIndex; i--) 
		array[i + 1] = array[i];

	array[startIndex] = 0;
}

int main()
{
	randomState = time(NULL);

	float growingArray[MAX_SIZE_OF_ARRAY];
	float descendingArray[MAX_SIZE_OF_ARRAY];
	float combinedArray[MAX_SIZE_OF_ARRAY * 2];

	unsigned int growingArraySize = 0;
	unsigned int descendingArraySize = 0;
	unsigned int combinedArraySize = 0;

	//Task 2 (number 3)
	printf("Define growing array:\n");
	userInputArray(&growingArray[0], &growingArraySize, GROWING_ARRAY);

	printf("\nDefine descending array:\n");
	userInputArray(&descendingArray[0], &descendingArraySize, DESCENDING_ARRAY);

	combinedArraySize = growingArraySize + descendingArraySize;

	for (int i = 0; i < growingArraySize; i++)
		combinedArray[i] = growingArray[i];
	for (int i = 0; i < descendingArraySize; i++)
		combinedArray[growingArraySize + i] = descendingArray[i];

	for (int i = 0; i < combinedArraySize; i++)
		for (int j = 0; j < combinedArraySize - i - 1; j++)
			if (combinedArray[j] > combinedArray[j + 1])
			{
				float tempElementValue = combinedArray[j];

				combinedArray[j] = combinedArray[j + 1];
				combinedArray[j + 1] = tempElementValue;
			}

	printf("\nCombined array (growing array + descending array) sorted ascending:\n");
	for (int i = 0; i < combinedArraySize; i++)
		printf("[%i]\t%.3f\n", i, combinedArray[i]);

	return 0;
}