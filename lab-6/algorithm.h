#pragma once

void swap(int* firstElementPtr, int* secondElementPtr);
void swapRowsInMatrix(int* firstRow, int* secondRow, unsigned int columnsCount);

int getCountOfEvenElementsInArray(int* array, unsigned int arraySize);

void quickSort(int* array, int left, int right, int step);
void bubleSort(int* array, unsigned int arraySize, int step);