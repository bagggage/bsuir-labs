#include "mylib.h"

int main()
{
	// Task 1 (number 5)
	/*{
		float legA, legB;

		printf("Enter the legs of a right triangle: ");

		while (scanf_s("%f %f", &legA, &legB) < 2)
			throwErrorAndClearInputBuffer();

		float hypotenuse = sqrt((legA * legA) + (legB * legB));
		float area = (legA * legB) / 2;

		printf("\nHypotenuse of the triangle = %.2f\n", hypotenuse);
		printf("Area of the triangle = %.2f\n", area);
		
		return 0;
	}*/

	//Task 2 (number 3)
	/*{
		int k;

		printf("\nEnter number: ");

		while (scanf_s("%i", &k) < 1)
			throwErrorAndClearInputBuffer();

		printf("Number %i is %s\n", k, (k % 2 == 0 ? "even" : "odd"));

		return 0;
	}*/

	//Task 3 (number 3)
	/*{
		float a, b, c;
		int countOfReciprocalsNumberPairs = 0;

		printf("\n\nEnter three numbers (a, b, c): ");

		while (scanf_s("%f %f %f", &a, &b, &c) < 3)
			throwErrorAndClearInputBuffer();

		if (a * b == 1)
			countOfReciprocalsNumberPairs++;
		if (a * c == 1)
			countOfReciprocalsNumberPairs++;
		if (b * c == 1)
			countOfReciprocalsNumberPairs++;

		printf("Count of reciprocals number pairs = %i\n", countOfReciprocalsNumberPairs);

		return 0;
	}*/

	//Task 4 (number 3)
	{
		char timeOfYear[8] = { '\0' };

		printf("\nEnter time of year in format - summer/autumn/winter/spring : ");

		while (scanf_s("%s", timeOfYear, 8) < 1)
			throwErrorAndClearInputBuffer();

		//Summer
		if (isStringEquals(timeOfYear, "summer"))
			printf("\t-June\n\t-July\n\t-August\n");
		//Autumn
		else if (isStringEquals(timeOfYear, "autumn"))
			printf("\t-September\n\t-October\n\t-November\n");
		//Winter
		else if (isStringEquals(timeOfYear, "winter"))
			printf("\t-December\n\t-January\n\t-February\n");
		//Spring
		else if (isStringEquals(timeOfYear, "spring"))
			printf("\t-March\n\t-April\n\t-May\n");
		//Other input
		else
			printf("Unknown format\n");

		return 0;
	}
}