#include <stdio.h>

#define MATRIX_MAX_SIZE 100

//colors
#define DEFAULT 0
#define YELLOW 1
#define GREEN 2

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

void printMatrixWithHighlightedArea(float* matrixPtr, int matrixSize, int highlightColor)
{
	int columnAreaStartIndex = matrixSize % 2 == 0 ? (matrixSize / 2) - 1 : matrixSize / 2;
	int columnAreaEndIndex = matrixSize / 2;

	for (int row = 0; row < matrixSize; row++)
	{
		printf("\t");

		for (int col = 0; col < matrixSize; col++)
		{
			if (row >= matrixSize / 2 && col >= columnAreaStartIndex && col <= columnAreaEndIndex)
				setTextColor(highlightColor);
			else
				setTextColor(DEFAULT);

			printf(" [%.3f]", matrixPtr[row * MATRIX_MAX_SIZE + col]);

			printf(col == matrixSize - 1 ? "\n" : ",");
		}

		if (row >= matrixSize / 2) 
		{
			columnAreaEndIndex++;
			columnAreaStartIndex--;
		}

		setTextColor(DEFAULT);
	}
}

void userInputMatrix(float* matrixPtr, int* matrixSize)
{
	char choice = '\0';

	printf("Enter size of NxN matrix: ");

	while (*matrixSize < 1 || *matrixSize > 100)
	{
		while (scanf_s("%d", matrixSize) < 1 || getchar() != '\n')
			throwErrorAndClearInputBuffer("Incorrect input, please try enter data again in format (rows columns): ");

		if (*matrixSize < 1 || *matrixSize > 100)
			throwErrorAndClearInputBuffer("Size out of range (1 - 100), please enter correct data: ");
	}

	printf("Do you want to enter matrix elements? (Otherwise elements will genereted by random in range from -10^2 to 10^2)\n(Y/N): ");

	randomState = time(0);

	rewind(stdin);

	while (scanf_s("%c", &choice, 1) < 1 || getchar() != '\n' || (choice != 'N' && choice != 'Y'))
		throwErrorAndClearInputBuffer("Incorrect input, enter answer in format (Y/N): ");
 
  char lastChar;

	if (choice == 'Y')
		for (int row = 0; row < *matrixSize; row++)
			for (int col = 0; col < *matrixSize; col++)
				while (scanf_s("%f", &matrixPtr[row * MATRIX_MAX_SIZE + col]) < 1 || ((lastChar = getchar()) != '\n' && lastChar != ' '))
				{
					throwErrorAndClearInputBuffer("Incorrect input, input should be a floating point number\n");

					printf("Current matrix state: \n");
					for (int k = 0; k <= row; k++)
					{
						int lastColumnIndex = (k == row ? col : *matrixSize);

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
		for (int row = 0; row < *matrixSize; row++)
			for (int col = 0; col < *matrixSize; col++)
				matrixPtr[row * MATRIX_MAX_SIZE + col] = randomRange(0, 1) > 0.5f ? randomRange(0, 100) : -randomRange(0, 100);
}

int main()
{
	float matrix[MATRIX_MAX_SIZE][MATRIX_MAX_SIZE];
	int matrixSize = 0;

	userInputMatrix(matrix, &matrixSize);

	printf("\nCurrent matrix state:\n");
	printMatrixWithHighlightedArea(matrix, matrixSize, GREEN);

	float sumOfElementsInArea = 0;

	int columnAreaStartIndex = matrixSize % 2 == 0 ? (matrixSize / 2) - 1 : matrixSize / 2;
	int columnAreaEndIndex = matrixSize / 2;

	for (int row = matrixSize / 2; row < matrixSize; row++)
	{
		for (int col = columnAreaStartIndex; col <= columnAreaEndIndex; col++)
			sumOfElementsInArea += matrix[row][col];

		columnAreaEndIndex++;
		columnAreaStartIndex--;
	}

	printf("\nSum of elements in bottom triangle area = %.3f\n", sumOfElementsInArea);

	return 0;
}