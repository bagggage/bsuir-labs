#pragma once

#define OUTPUT_OPTIMIZATION_ARRAY_SIZE 100 

int* initArray(unsigned int size);
int* copyArray(int* originalArray, unsigned int arraySize);

void deleteElementInArray(int** array, unsigned int* size, unsigned int indexOfElement);
void printArray(int* array, unsigned int size);