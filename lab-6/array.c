#include "array.h"

#include <stdio.h>
#include <stdlib.h>

int* initArray(unsigned int size)
{
    return malloc(size * sizeof(int));
}

int* copyArray(int* originalArray, unsigned int arraySize) 
{
    int* copiedArray = initArray(arraySize);

    for (int i = 0; i < arraySize; i++)
        copiedArray[i] = originalArray[i];

    return copiedArray;
}

void deleteElementInArray(int** array, unsigned int* size, unsigned int indexOfElement) 
{
    *size = *size - 1;

    for (int i = indexOfElement; i < *size; i++)
        (*array)[i] = (*array)[i + 1];

    *array = realloc(*array, *size * sizeof(int));
}

void printArray(int* array, unsigned int size) 
{
    char* buffer = NULL;

    if (size >= OUTPUT_OPTIMIZATION_ARRAY_SIZE)
    {
        buffer = calloc(size, sizeof(char) * 5);
        setvbuf(stdout, buffer, _IOFBF, size * sizeof(char) * 5);
    }

    for (int i = 0; i < size; i++)
        printf("%4d ", array[i]);

    if (size >= OUTPUT_OPTIMIZATION_ARRAY_SIZE) 
    {
        setvbuf(stdout, buffer, _IONBF, size * sizeof(char) * 5);
        free(buffer);
    }

    printf("\n");
}
