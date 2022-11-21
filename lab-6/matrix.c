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

void destructMatrix(int** matrix)
{
	free(matrix[0]);
	free(matrix);
}

void printMatrix(int** matrix, unsigned int* matrixSize) 
{
	for (int row = 0; row < matrixSize[ROWS]; row++)
	{
		for (int col = 0; col < matrixSize[COLUMNS]; col++)
			printf("[%4d] ", matrix[row][col]);
		
		printf("\n");
	}
}