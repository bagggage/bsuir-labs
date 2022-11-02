#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"
#include "random.h"

#define MATRIX_MAX_SIZE 100

void throwErrorAndClearInputBuffer(char* errorText)
{
	printf("%s", errorText);
	rewind(stdin);
}

void printMatrixUpToCertainElement(int** matrix, unsigned int* size, unsigned int rowIndexOfCertainElement, unsigned int colIndexOfCertainElement)
{
	for (int k = 0; k <= rowIndexOfCertainElement; k++)
	{
		int lastColumnIndex = (k == rowIndexOfCertainElement ? colIndexOfCertainElement : size[COLUMNS]);

		printf("\t");

		for (int g = 0; g < lastColumnIndex; g++)
		{
			printf(" [%d]", matrix[k][g]);

			printf(g == lastColumnIndex - 1 ? "\n" : ",");
		}
	}

	printf("\n");
}

void generateMatrixElementsByRandom(int** matrix, unsigned int* size)
{
	for (int row = 0; row < size[ROWS]; row++)
		for (int col = 0; col < size[COLUMNS]; col++)
			matrix[row][col] = randomRange(-100, 100);
}

void readMatrixElementsFromInput(int** matrix, unsigned int* size)
{
	char lastChar;

	for (int row = 0; row < size[ROWS]; row++)
		for (int col = 0; col < size[COLUMNS]; col++)
			while (scanf_s("%d", &matrix[row][col]) < 1 ||
				((lastChar = getchar()) != '\n' && lastChar != ' ') ||
				matrix[row][col] == 100)
			{
				if (matrix[row][col] == 100)
					throwErrorAndClearInputBuffer("Value '100' is reserved by programm\n");
				else
					throwErrorAndClearInputBuffer("Incorrect input, input should be a integer number\n");

				printf("Current matrix state: \n");
				printMatrixUpToCertainElement(matrix, size, row, col);

				printf("Enter element [%d][%d] again: ", row, col);
			}
}

char isMatrixSizeOutOfRange(int* size)
{
	if (size[ROWS] > MATRIX_MAX_SIZE || size[COLUMNS] > MATRIX_MAX_SIZE ||
		size[ROWS] < 1 || size[COLUMNS] < 1)
		return 1;
	else
		return 0;
}

void userInputMatrix(int*** matrixPtr, unsigned int* size)
{
	printf("Enter size of matrix in format (rows columns): ");

	while (isMatrixSizeOutOfRange(size))
	{
		while (scanf_s("%d %d", &size[ROWS], &size[COLUMNS]) < 2 || getchar() != '\n')
			throwErrorAndClearInputBuffer("Incorrect input, please try enter data again in format (rows columns): ");

		if (isMatrixSizeOutOfRange(size))
			throwErrorAndClearInputBuffer("Size out of range (1 - 100), please enter correct data: ");
	}

	*matrixPtr = initMatrix(size);

	char choice;

	printf("Do you want to enter matrix elements? (Otherwise elements will genereted by random in range from -10^2 to 10^2)\n(Y/N): ");

	while (scanf_s("%c", &choice, 1) < 1 || getchar() != '\n' || (choice != 'N' && choice != 'Y'))
		throwErrorAndClearInputBuffer("Incorrect input, enter answer in format (Y/N): ");

	if (choice == 'Y')
		readMatrixElementsFromInput(*matrixPtr, size);
	else
		generateMatrixElementsByRandom(*matrixPtr, size);
}

int main()
{
	unsigned int matrixSize[] = { 0, 0 };
	int** matrix;

	setRandomSeed(time(0));

	userInputMatrix(&matrix, matrixSize);

	printf("\nCurrent matrix state:\n");
	printMatrix(matrix, matrixSize);

	int k;

	printf("\nEnter offset coefficient: ");

	while (scanf_s("%d", &k) < 1 || getchar() != '\n' || k < 0)
		throwErrorAndClearInputBuffer("Incorrect input, input should be a positive integer number: ");

	for (int row = 0; row < matrixSize[ROWS]; row++)
		for (int i = 0; i < k; i++)
		{
			int tempValue;
			
			for (int col = 0; col < matrixSize[COLUMNS] - 1; col++)
			{
				if (col == 0)
				{
					tempValue = matrix[row][matrixSize[COLUMNS] - 1];
					matrix[row][matrixSize[COLUMNS] - 1] = matrix[row][col];
				}

				if (col != matrixSize[COLUMNS] - 2)
					matrix[row][col] = matrix[row][col + 1];
				else
					matrix[row][col] = tempValue;
			}
		}

	printf("\nChanged matrix state:\n");
	printMatrix(matrix, matrixSize);

	destructMatrix(matrix);

	return 0;
}