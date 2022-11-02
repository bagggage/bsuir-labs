#include "matrix.h"

#include <stdlib.h>
#include <stdio.h>

int** initMatrix(unsigned int* matrixSize)
{
	int** matrix = malloc(matrixSize[ROWS] * sizeof(int*));
	int* matrixElements = malloc(matrixSize[ROWS] * matrixSize[COLUMNS] * sizeof(int));

	for (int i = 0; i < matrixSize[ROWS]; i++)
		matrix[i] = matrixElements + (i * matrixSize[COLUMNS]);

	return matrix;
}

void destructMatrix(int** matrixPtr)
{
	free(matrixPtr[0]);
	free(matrixPtr);
}

void deleteElementInMatrix(int** matrix, unsigned int* matrixSize, unsigned int rowIndexOfElement, unsigned int colIndexOfElement) 
{
	int countOfElementsInMatrix = 0;
	int lineIndexOfElement = 0;

	for (int row = 0; row < matrixSize[ROWS]; row++)
	{
		for (int col = 0; matrix[row][col] != 100; col++)
		{
			if (row == rowIndexOfElement && col == colIndexOfElement)
				lineIndexOfElement = countOfElementsInMatrix;

			countOfElementsInMatrix++;
		}

		countOfElementsInMatrix++;
	}

	for (int i = lineIndexOfElement; i < countOfElementsInMatrix - 1; i++)
		matrix[0][i] = matrix[0][i + 1];

	int* oldMatrixElementsPointer = matrix[0];

	matrix[0] = realloc(matrix[0], (countOfElementsInMatrix - 1) * sizeof(int));

	for (int row = 1; row < matrixSize[ROWS]; row++)
	{
		if (row > rowIndexOfElement)
			matrix[row] = matrix[0] + (matrix[row] - oldMatrixElementsPointer - 1);
		else
			matrix[row] = matrix[0] + (matrix[row] - oldMatrixElementsPointer);
	}
}

void printMatrix(int** matrix, unsigned int* matrixSize) 
{
	for (int row = 0; row < matrixSize[ROWS]; row++)
	{
		if (matrix[row][0] == 100)
			continue;

		printf("\t");

		for (int col = 0; col < matrixSize[COLUMNS]; col++)
		{
			if (matrix[row][col] == 100)
				break;

			printf("[%d] ", matrix[row][col]);
		}

		
		printf("\n");
	}
}