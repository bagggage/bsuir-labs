//#include <stdio.h>
//#include <stdlib.h>
//
//#include "matrix.h"
//#include "random.h"
//
//#define MATRIX_MAX_SIZE 100
//
////colors
//#define DEFAULT 0
//#define YELLOW 1
//
//void throwErrorAndClearInputBuffer(char* errorText)
//{
//	printf("%s", errorText);
//	rewind(stdin);
//}
//
//void setTextColor(int colorID) 
//{
//	switch (colorID)
//	{
//	case YELLOW:
//		printf("\033[1;33m");
//		break;
//	default:
//		printf("\033[0m");
//		break;
//	}
//}
//
//void printMatrixWithHighlightedNegativeElements(int** matrix, unsigned int* size, int colorID)
//{
//	for (int row = 0; row < size[ROWS]; row++)
//	{
//		printf("\t");
//
//		for (int col = 0; col < size[COLUMNS]; col++)
//		{
//			if (matrix[row][col] < 0)
//				setTextColor(colorID);
//
//			printf("[%d] ", matrix[row][col]);
//
//			setTextColor(DEFAULT);
//		}
//
//		printf("\n");
//	}
//}
//
//void printMatrixUpToCertainElement(int** matrix, unsigned int* size, unsigned int rowIndexOfCertainElement, unsigned int colIndexOfCertainElement) 
//{
//	for (int k = 0; k <= rowIndexOfCertainElement; k++)
//	{
//		int lastColumnIndex = (k == rowIndexOfCertainElement ? colIndexOfCertainElement : size[COLUMNS] - 1);
//
//		printf("\t");
//
//		for (int g = 0; g < lastColumnIndex; g++)
//		{
//			printf(" [%d]", matrix[k][g]);
//
//			printf(g == lastColumnIndex - 1 ? "\n" : ",");
//		}
//	}
//
//	printf("\n");
//}
//
//void setEndOfRows(int** matrix, unsigned int* size) 
//{
//	for (int row = 0; row < size[ROWS]; row++)
//		matrix[row][size[COLUMNS] - 1] = 100;
//}
//
//void generateMatrixElementsByRandom(int** matrix, unsigned int* size)
//{
//	for (int row = 0; row < size[ROWS]; row++)
//		for (int col = 0; matrix[row][col] != 100; col++)
//			matrix[row][col] = randomRange(-100, 100);
//}
//
//void readMatrixElementsFromInput(int** matrix, unsigned int* size)
//{
//	char lastChar;
//
//	for (int row = 0; row < size[ROWS]; row++)
//		for (int col = 0; matrix[row][col] != 100; col++)
//			while (scanf_s("%d", &matrix[row][col]) < 1 ||
//				((lastChar = getchar()) != '\n' && lastChar != ' ') ||
//				matrix[row][col] == 100)
//			{
//				if (matrix[row][col] == 100)
//					throwErrorAndClearInputBuffer("Value '100' is reserved by programm\n");
//				else
//					throwErrorAndClearInputBuffer("Incorrect input, input should be a integer number\n");
//
//				printf("Current matrix state: \n");
//				printMatrixUpToCertainElement(matrix, size, row, col);
//
//				printf("Enter element [%d][%d] again: ", row, col);
//			}
//}
//
//char isMatrixSizeOutOfRange(int* size)
//{
//	if (size[ROWS] > MATRIX_MAX_SIZE || size[COLUMNS] > MATRIX_MAX_SIZE ||
//		size[ROWS] < 1 || size[COLUMNS] < 1)
//		return 1;
//	else
//		return 0;
//}
//
//void userInputMatrix(int*** matrixPtr, unsigned int* size) 
//{
//	printf("Enter size of matrix in format (rows columns): ");
//
//	while (isMatrixSizeOutOfRange(size))
//	{
//		while (scanf_s("%d %d", &size[ROWS], &size[COLUMNS]) < 2 || getchar() != '\n')
//			throwErrorAndClearInputBuffer("Incorrect input, please try enter data again in format (rows columns): ");
//
//		if (isMatrixSizeOutOfRange(size))
//			throwErrorAndClearInputBuffer("Size out of range (1 - 100), please enter correct data: ");
//	}
//
//	size[COLUMNS]++;
//
//	*matrixPtr = initMatrix(size);
//	setEndOfRows(*matrixPtr, size);
//
//	char choice;
//
//	printf("Do you want to enter matrix elements? (Otherwise elements will genereted by random in range from -10^2 to 10^2)\n(Y/N): ");
//
//	while (scanf_s("%c", &choice, 1) < 1 || getchar() != '\n' || (choice != 'N' && choice != 'Y'))
//		throwErrorAndClearInputBuffer("Incorrect input, enter answer in format (Y/N): ");
//
//	if (choice == 'Y')
//		readMatrixElementsFromInput(*matrixPtr, size);
//	else
//		generateMatrixElementsByRandom(*matrixPtr, size);
//}
//
//int main() 
//{
//	unsigned int matrixSize[] = { 0, 0 };
//	int** matrix;
//
//	setRandomSeed(time(0));
//
//	userInputMatrix(&matrix, &matrixSize);
//
//	printf("\nCurrent matrix state:\n");
//	printMatrixWithHighlightedNegativeElements(matrix, matrixSize, YELLOW);
//
//	for (int row = 0; row < matrixSize[ROWS]; row++)
//		for (int col = 0; col < matrixSize[COLUMNS] && matrix[row][col] != 100; col++)
//			if (matrix[row][col] < 0) 
//			{
//				deleteElementInMatrix(matrix, matrixSize, row, col);
//				col--;
//			}
//
//	printf("\nChanged matrix state:\n");
//	printMatrix(matrix, matrixSize);
//
//	printf("\n");
//
//	for (int row = 0; row < matrixSize[ROWS]; row++)
//	{
//		printf("\t");
//
//		int tempColVal = 0;
//
//		for (int col = 0; matrix[row][col] != 100; col++)
//		{
//			tempColVal = col + 1;
//			printf("[%d] ", matrix[row][col]);
//		}
//
//		printf("[%d] ", matrix[row][tempColVal]);
//
//		printf("\n");
//	}
//
//	destructMatrix(matrix);
//
//	return 0;
//}