//#include <stdio.h>
//#include <stdlib.h>
//
//#include "array.h"
//
//void throwErrorAndClearInputBuffer(char* errorText) 
//{
//	printf("%s", errorText);
//	rewind(stdin);
//}
//
//void userInputArray(int** arrayPtr, unsigned int* arraySize) 
//{
//	printf("Enter array size (1 - 100): ");
//
//	while (scanf_s("%u", arraySize) < 1 || getchar() != '\n' || *arraySize < 1 || *arraySize > 100)
//	{
//		if (*arraySize < 1 || *arraySize > 100)
//			throwErrorAndClearInputBuffer("Size out of range, please try enter size again (1 - 100): ");
//		else
//			throwErrorAndClearInputBuffer("Incorrect input, please try enter size again (1 - 100): ");
//	}
//
//	*arrayPtr = initArray(*arraySize);
//
//	char lastChar;
//
//	printf("Enter array elements: ");
//
//	for (int i = 0; i < *arraySize; i++)
//		while (scanf_s("%u", &(*arrayPtr)[i]) < 1 || ((lastChar = getchar()) != '\n' && lastChar != ' '))
//			throwErrorAndClearInputBuffer("Incorrect input, please try enter element again: ");
//}
//
//int main()
//{
//	unsigned int arraySize;
//	int* array;
//
//	userInputArray(&array, &arraySize);
//	printArray(array, arraySize);
//
//	for (int i = 0; i < arraySize; i++)
//		for (int j = i + 1; j < arraySize; j++)
//			if (array[j] == array[i])
//			{
//				deleteElementInArray(&array, &arraySize, j);
//				j--;
//			}
//
//	printArray(array, arraySize);
//
//	free(array);
//
//	return 0;
//}