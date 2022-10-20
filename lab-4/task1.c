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
	for (int i = 0; i < MATRIX_SIZE; i++)
	{
		printf("\t");

		if ((i + 1) % 2 == 0)
			setTextColor(GREEN);

		for (int j = 0; j < MATRIX_SIZE; j++)
		{
			printf(" [%.3f]", matrixPtr[i * MATRIX_SIZE + j]);

			printf(j == MATRIX_SIZE - 1 ? "\n" : ",");
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

	if (choice == 'Y')
		for (int i = 0; i < MATRIX_SIZE; i++)
			for (int j = 0; j < MATRIX_SIZE; j++)
				while (scanf_s("%f", &matrixPtr[i * MATRIX_SIZE + j]) < 1 || getchar() != '\n')
				{
					throwErrorAndClearInputBuffer("Incorrect input, input should be a floating point number\n");

					printf("Current matrix state: \n");
					for (int k = 0; k <= i; k++)
					{
						int lastColumnIndex = k == i ? j : MATRIX_SIZE;

						printf("\t");

						for (int g = 0; g < lastColumnIndex; g++)
						{
							printf(" [%.3f]", matrixPtr[(k * MATRIX_SIZE) + g]);

							printf(g == lastColumnIndex - 1 ? "\n" : ",");
						}
					}

					printf("Enter element [%i][%i] again: ", i, j);
				}
	else
		for (int i = 0; i < MATRIX_SIZE; i++)
			for (int j = 0; j < MATRIX_SIZE; j++)
				matrixPtr[i * MATRIX_SIZE + j] = randomRange(0, 1) > 0.5f ? randomRange(0, 100) : -randomRange(0, 100);
}

int main()
{
	float matrix[MATRIX_SIZE][MATRIX_SIZE];

	float sumOfElementInEvenStrings = 0;
	float sumOfElementInOddStrings = 0;

	userInputMatrix(matrix);

	printf("\nCurrent matrix state:\n");
	printMatrixWithHighlightedEvenRows(matrix);

	for (int i = 0; i < MATRIX_SIZE; i++)
	{
		char isStringEven = (i + 1) % 2 == 0 ? 1 : 0;

		for (int j = 0; j < MATRIX_SIZE; j++)
			if (isStringEven)
				sumOfElementInEvenStrings += matrix[i][j];
			else
				sumOfElementInOddStrings += matrix[i][j];
	}

	printf("\nSum of elements in odd strings = %.3f", sumOfElementInOddStrings);
	printf("\nSum of elements in even strings = %.3f\n", sumOfElementInEvenStrings);

	return 0;
}