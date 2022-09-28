#include <stdio.h>

#define MAX_SIZE_OF_ARRAY 100

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
	return (randomByte() % ((int)(max * 1000) + -(int)(min * 1000)) + (int)(min * 1000)) / 1000.f;
}

void throwErrorAndClearInputBuffer(const char* errorMsg)
{
	printf(errorMsg);
	rewind(stdin);
}

void userInputArray(float* array, unsigned int* arraySizePtr)
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

	printf("\nDo you want to enter array elements? (Otherwise elements will genereted by random in range from -10^3 to 10^3)\n(Y/N): ");

	rewind(stdin);

	while (scanf_s("%c", &choice, 1) < 1 || (choice != 'N' && choice != 'Y'))
		throwErrorAndClearInputBuffer("Incorrect input, enter answer in format (Y/N): ");

	if (choice == 'Y')
		for (int i = 0; i < *arraySizePtr; i++)
			while (scanf_s("%f", &array[i]) < 1)
			{
				throwErrorAndClearInputBuffer("Incorrect input, input should be a floating point number\n");

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
			array[i] = randomRange(0, 1) > 0.5f ? randomRange(0, 1000) : -randomRange(0, 1000);

	printf("\nCurrent array state:\n");

	for (int j = 0; j < *arraySizePtr; j++)
		printf("[%i]\t%.3f\n", j, array[j]);
}

void moveElementsInArray(float* array, unsigned int arraySize, unsigned int startIndex) 
{
	for (int i = arraySize - 1; i >= startIndex; i--) 
		array[i + 1] = array[i];

	array[startIndex] = 0;
}

int main()
{
	float array[MAX_SIZE_OF_ARRAY * 2] = { 0 };

	unsigned int arraySize = 0;

	//Task 2 (number 3)
	userInputArray(&array[0], &arraySize);

	for (int i = 0; i < arraySize; i++)
	{
		if (array[i] < 0)
		{
			if (i + 1 < arraySize)
				moveElementsInArray(&array[0], arraySize, i + 1);

			array[i + 1] = array[i];
			arraySize++;
			i++;
		}
	}

	printf("\nState of array after inserting copies of negative values\n");

	for (int i = 0; i < arraySize; i++)
		printf("[%i]\t%.3f\n", i, array[i]);

	return 0;
}