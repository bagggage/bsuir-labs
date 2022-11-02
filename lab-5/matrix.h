#pragma once

//matrix size
#define ROWS 0
#define COLUMNS 1

int** initMatrix(unsigned int* size);
void destructMatrix(int** matrixPtr);

void deleteElementInMatrix(int** matrix, unsigned int* matrixSize, unsigned int rowIndexOfElement, unsigned int colIndexOfElement);
void printMatrix(int** matrix, unsigned int* size);