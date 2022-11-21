#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "matrix.h"
#include "random.h"
#include "algorithm.h"

#define MATRIX_MAX_SIZE 1000

//colors
#define DEFAULT 0
#define YELLOW 1

void throwErrorAndClearInputBuffer(char* errorMsg)
{
	printf(errorMsg);
	rewind(stdin);
}

void setTextColor(int colorID)
{
	switch (colorID)
	{
	case YELLOW:
		printf("\033[1;33m");
		break;
	default:
		printf("\033[0m");
		break;
	}
}

void generateMatrixElementsByRandom(int** matrix, unsigned int* size)
{
	for (int row = 0; row < size[ROWS]; row++)
		for (int col = 0; col < size[COLUMNS]; col++)
			matrix[row][col] = randomRange(-1000, 1000);
}

void printMatrixWithHighlightedEvenElements(int** matrix, unsigned int* matrixSize, int colorID) 
{
	for (int row = 0; row < matrixSize[ROWS]; row++)
	{
		for (int col = 0; col < matrixSize[COLUMNS]; col++)
		{
			if (matrix[row][col] % 2 == 0)
				setTextColor(colorID);

			printf("[%4d] ", matrix[row][col]);

			setTextColor(DEFAULT);
		}

		printf("\n");
	}
}

void printMatrixUpToCertainElement(int** matrix, unsigned int* size, unsigned int rowIndexOfCertainElement, unsigned int colIndexOfCertainElement)
{
	for (int k = 0; k <= rowIndexOfCertainElement; k++)
	{
		int lastColumnIndex = (k == rowIndexOfCertainElement ? colIndexOfCertainElement : size[COLUMNS]);

		printf("\t");

		for (int g = 0; g < lastColumnIndex; g++)
		{
			printf(" [%4d]", matrix[k][g]);

			printf(g == lastColumnIndex - 1 ? "\n" : ",");
		}
	}

	printf("\n");
}

void readMatrixElementsFromInput(int** matrix, unsigned int* size)
{
	char lastChar;

	for (int row = 0; row < size[ROWS]; row++)
		for (int col = 0; col < size[COLUMNS]; col++)
			while (scanf_s("%d", &matrix[row][col]) < 1 ||
				((lastChar = getchar()) != '\n' && lastChar != ' '))
			{
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

void userInputMatrix(int*** matrixPtr, unsigned int* matrixSize)
{
	printf("Enter size of matrix in format (rows columns): ");

	while (isMatrixSizeOutOfRange(matrixSize))
	{
		while (scanf_s("%d %d", &matrixSize[ROWS], &matrixSize[COLUMNS]) < 2 || getchar() != '\n')
			throwErrorAndClearInputBuffer("Incorrect input, please try enter data again in format (rows columns): ");

		if (isMatrixSizeOutOfRange(matrixSize))
			throwErrorAndClearInputBuffer("Size out of range (1 - 1000), please enter correct data: ");
	}

	*matrixPtr = initMatrix(matrixSize);

	char choice;

	printf("Do you want to enter matrix elements? (Otherwise elements will genereted by random in range from -10^3 to 10^3)\n(Y/N): ");

	while (scanf_s("%c", &choice, 1) < 1 || getchar() != '\n' || (choice != 'N' && choice != 'Y'))
		throwErrorAndClearInputBuffer("Incorrect input, enter answer in format (Y/N): ");

	if (choice == 'Y')
		readMatrixElementsFromInput(*matrixPtr, matrixSize);
	else
		generateMatrixElementsByRandom(*matrixPtr, matrixSize);
}

int main()
{
	unsigned int matrixSize[] = { 0, 0 };
	int** matrix;

	setRandomSeed(time(0));

	userInputMatrix(&matrix, matrixSize);
	printf("\nCurrent state of matrix:\n");
	printMatrixWithHighlightedEvenElements(matrix, matrixSize, YELLOW);

	for (int i = 1; i < matrixSize[ROWS]; i++)
		for (int j = i; j > 0 &&
			getCountOfEvenElementsInArray(matrix[j - 1], matrixSize[COLUMNS]) <
			getCountOfEvenElementsInArray(matrix[j], matrixSize[COLUMNS]); j--)
			swapRowsInMatrix(matrix[j - 1], matrix[j], matrixSize[COLUMNS]);

	printf("\nState of matrix after sotring rows:\n");
	printMatrixWithHighlightedEvenElements(matrix, matrixSize, YELLOW);

	destructMatrix(matrix);

	return 0;
}