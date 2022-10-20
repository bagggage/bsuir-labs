#include <stdio.h>

#define MATRIX_MAX_SIZE 100

//colors
#define DEFAULT 0
#define YELLOW 1
#define GREEN 2
//matrix size
#define ROWS 0
#define COLUMNS 1

long long randomState;
const long long randomMax = 4294967296;

unsigned int randomByte()
{
	randomState = 214013 * randomState + 2531011;
	randomState ^= randomState >> 15;

	return (unsigned int)(randomState % randomMax);
}

float randomRange(float min, float max)
{
	return (randomByte() % ((int)(max * 1000) - (int)(min * 1000)) + (int)(min * 1000)) / 1000.f;
}

void setTextColor(int colorID)
{
	switch (colorID)
	{
	case YELLOW:
		printf("\033[1;33m");
		break;
	case GREEN:
		printf("\033[0;32m");
		break;
	default:
		printf("\033[0m");
		break;
	}
}

void throwErrorAndClearInputBuffer(const char* errorMsg)
{
	printf(errorMsg);
	rewind(stdin);
}

//Returns index of sorted ascending string, if there is no such string, returns -1
int findSortedAscendingRow(float* matrixPtr, int* matrixSize) 
{
	int sortedAscendingRow = -1;

	for (int row = 0; row < matrixSize[ROWS]; row++)
	{
		for (int col = 0; col < matrixSize[COLUMNS]; col++)
		{
			if (col > 0)
				if (matrixPtr[row * MATRIX_MAX_SIZE + col] >= matrixPtr[row * MATRIX_MAX_SIZE + col - 1])
				{
					if (col == matrixSize[COLUMNS] - 1)
						sortedAscendingRow = row;
				}

				else
				{
					break;
				}
		}

		if (sortedAscendingRow != -1)
			break;
	}

	return sortedAscendingRow;
}

void printMatrixWithHighlightedRow(float* matrixPtr, int* matrixSize, int indexOfHighlightedRow, int highlightColor) 
{
	for (int row = 0; row < matrixSize[ROWS]; row++)
	{
		printf("\t");

		if (row == indexOfHighlightedRow)
			setTextColor(highlightColor);

		for (int col = 0; col < matrixSize[COLUMNS]; col++)
		{
			printf(" [%.3f]", matrixPtr[row * MATRIX_MAX_SIZE + col]);

			printf(col == matrixSize[COLUMNS] - 1 ? "\n" : ",");
		}

		setTextColor(DEFAULT);
	}
}

char isMatrixSizeOutOfRange(int* matrixSize) 
{
	if (matrixSize[ROWS] > MATRIX_MAX_SIZE || matrixSize[COLUMNS] > MATRIX_MAX_SIZE ||
		matrixSize[ROWS] < 1 || matrixSize[COLUMNS] < 1)
		return 1;
	else
		return 0;
}

void userInputMatrix(float* matrixPtr, int* matrixSize)
{
	char choice = '\0';

	printf("Enter size of matrix in format (rows columns): ");

	while (isMatrixSizeOutOfRange(matrixSize))
	{
		while (scanf_s("%d %d", &matrixSize[ROWS], &matrixSize[COLUMNS]) < 2 || getchar() != '\n')
			throwErrorAndClearInputBuffer("Incorrect input, please try enter data again in format (rows columns): ");

		if (isMatrixSizeOutOfRange(matrixSize))
			throwErrorAndClearInputBuffer("Size out of range (1 - 100), please enter correct data: ");
	}

	printf("Do you want to enter matrix elements? (Otherwise elements will genereted by random in range from -10^2 to 10^2)\n(Y/N): ");

	randomState = time(0);

	rewind(stdin);

	while (scanf_s("%c", &choice, 1) < 1 || getchar() != '\n' || (choice != 'N' && choice != 'Y'))
		throwErrorAndClearInputBuffer("Incorrect input, enter answer in format (Y/N): ");
 
  char lastChar;

	if (choice == 'Y')
		for (int row = 0; row < matrixSize[ROWS]; row++)
			for (int col = 0; col < matrixSize[COLUMNS]; col++)
				while (scanf_s("%f", &matrixPtr[row * MATRIX_MAX_SIZE + col]) < 1 || ((lastChar = getchar()) != '\n' && lastChar != ' '))
				{
					throwErrorAndClearInputBuffer("Incorrect input, input should be a floating point number\n");

					printf("Current matrix state: \n");
					for (int k = 0; k <= row; k++)
					{
						int lastColumnIndex = (k == row ? col : matrixSize[COLUMNS]);

						printf("\t");

						for (int g = 0; g < lastColumnIndex; g++)
						{
							printf(" [%.3f]", matrixPtr[(k * MATRIX_MAX_SIZE) + g]);

							printf(g == lastColumnIndex - 1 ? "\n" : ",");
						}
					}

					printf("Enter element [%i][%i] again: ", row, col);
				}

	else
		for (int row = 0; row < matrixSize[ROWS]; row++)
			for (int col = 0; col < matrixSize[COLUMNS]; col++)
				matrixPtr[row * MATRIX_MAX_SIZE + col] = randomRange(0, 1) > 0.5f ? randomRange(0, 100) : -randomRange(0, 100);
}

int main()
{
	float matrix[MATRIX_MAX_SIZE][MATRIX_MAX_SIZE];
	int matrixSize[2] = { 0, 0 };

	userInputMatrix(matrix, matrixSize);

	int sortedAscendingRow = findSortedAscendingRow(matrix, matrixSize);

	printf("\nCurrent matrix state:\n");
	printMatrixWithHighlightedRow(matrix, matrixSize, sortedAscendingRow, GREEN);

	if (sortedAscendingRow != -1)
	{
		for (int col = 0; col < matrixSize[COLUMNS] / 2; col++)
		{
			float tempColumnValue = matrix[sortedAscendingRow][col];

			matrix[sortedAscendingRow][col] = matrix[sortedAscendingRow][matrixSize[COLUMNS] - col - 1];
			matrix[sortedAscendingRow][matrixSize[COLUMNS] - col - 1] = tempColumnValue;
		}	

		printf("\nMatrix state after changing:\n");
		printMatrixWithHighlightedRow(matrix, matrixSize, sortedAscendingRow, YELLOW);
	}

	else 
	{
		printf("\nThe matrix does not have a row sorted in ascending order\n");
	}

	return 0;
}