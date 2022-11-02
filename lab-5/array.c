#include "array.h"

#include <stdio.h>
#include <stdlib.h>

int* initArray(unsigned int size)
{
    return malloc(size * sizeof(int));
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
    for (int i = 0; i < size; i++)
        printf("%d ", array[i]);

    printf("\n");
}
