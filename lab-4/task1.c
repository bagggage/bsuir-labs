#include <stdio.h>

#define MATRIX_SIZE 6

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

void printMatrixWithHighlightedEvenRows(float* matrixPtr) 
{
	for (int row = 0; row < MATRIX_SIZE; row++)
	{
		printf("\t");

		if ((row + 1) % 2 == 0)
			setTextColor(GREEN);

		for (int col = 0; col < MATRIX_SIZE; col++)
		{
			printf(" [%.3f]", matrixPtr[row * MATRIX_SIZE + col]);

			printf(col == MATRIX_SIZE - 1 ? "\n" : ",");
		}

		setTextColor(DEFAULT);
	}
}

void userInputMatrix(float* matrixPtr)
{
	char choice = '\0';

	printf("Do you want to enter matrix elements? (Otherwise elements will genereted by random in range from -10^2 to 10^2)\n(Y/N): ");

	randomState = time(0);

	rewind(stdin);

	while (scanf_s("%c", &choice, 1) < 1 || getchar() != '\n' || (choice != 'N' && choice != 'Y'))
		throwErrorAndClearInputBuffer("Incorrect input, enter answer in format (Y/N): ");

	char lastChar;

	if (choice == 'Y')
		for (int row = 0; row < MATRIX_SIZE; row++)
			for (int col = 0; col < MATRIX_SIZE; col++)
				while (scanf_s("%f", &matrixPtr[row * MATRIX_SIZE + col]) < 1 || ((lastChar = getchar()) != '\n' && lastChar != ' '))
				{
					throwErrorAndClearInputBuffer("Incorrect input, input should be a floating point number\n");

					printf("Current matrix state: \n");
					for (int k = 0; k <= row; k++)
					{
						int lastColumnIndex = k == row ? col : MATRIX_SIZE;

						printf("\t");

						for (int g = 0; g < lastColumnIndex; g++)
						{
							printf(" [%.3f]", matrixPtr[(k * MATRIX_SIZE) + g]);

							printf(g == lastColumnIndex - 1 ? "\n" : ",");
						}
					}

					printf("Enter element [%i][%i] again: ", row, col);
				}
	else
		for (int row = 0; row < MATRIX_SIZE; row++)
			for (int col = 0; col < MATRIX_SIZE; col++)
				matrixPtr[row * MATRIX_SIZE + col] = randomRange(0, 1) > 0.5f ? randomRange(0, 100) : -randomRange(0, 100);
}

int main()
{
	float matrix[MATRIX_SIZE][MATRIX_SIZE];

	float sumOfSpecialElements = 0;

	userInputMatrix(matrix);

	printf("\nCurrent matrix state:\n");
	printMatrixWithHighlightedEvenRows(matrix);

	float tempMinElement;
	float tempMaxElement;

	for (int row = 0; row < MATRIX_SIZE; row++)
	{
		char isRowEven = (row + 1) % 2 == 0 ? 1 : 0;

		for (int col = 0; col < MATRIX_SIZE; col++)
			if (isRowEven)
			{
				if (col == 0 || matrix[row][col] > tempMaxElement)
					tempMaxElement = matrix[row][col];

				if (col == MATRIX_SIZE - 1)
					sumOfSpecialElements += tempMaxElement;
			}
			else
			{
				if (col == 0 || matrix[row][col] < tempMinElement)
					tempMinElement = matrix[row][col];

				if (col == MATRIX_SIZE - 1)
					sumOfSpecialElements += tempMinElement;
			}
	}

	printf("\nSum of min elements in odd and max elements in even strings = %.3f\n", sumOfSpecialElements);

	return 0;
}