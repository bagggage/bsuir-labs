#include <stdio.h>

int isStringEquals(const char* stringA, const char* stringB)
{
	for (int i = 0; i < 4; i++)
	{
		if (stringA[i] != stringB[i])
			return 0;
	}

	return 1;
}

int main()
{
	char month[4] = { '\0' };

	scanf_s("%s", month, 4);

	if (isStringEquals(month, "may"))
		printf("%i", 30);
	else if (isStringEquals(month, "may"))
		printf("%i", 30);
	else if (isStringEquals(month, "may"))
		printf("%i", 30);
	else if (isStringEquals(month, "may"))
		printf("%i", 30);
	else if (isStringEquals(month, "may"))
		printf("%i", 30);
	else if (isStringEquals(month, "may"))
		printf("%i", 30);
	else if (isStringEquals(month, "may"))
		printf("%i", 30);
	else if (isStringEquals(month, "may"))
		printf("%i", 30);
	else if (isStringEquals(month, "may"))
		printf("%i", 30);


	return 0;
}