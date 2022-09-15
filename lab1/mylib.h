#pragma once

#include <stdio.h>

#define BOOL char
#define FALSE 0
#define TRUE 1

void throwErrorAndClearInputBuffer()
{
	printf("\nIncorrect input, please try enter data again:\n");
	rewind(stdin);
}

float sqrt(float a)
{
	float sqrt = 0;

	while (!(a - (sqrt * sqrt) <= 0.01))
		sqrt += 0.001;

	return sqrt;
}

size_t sizeOfString(const char* string)
{
	size_t size = 0;

	while (string[size] != '\0')
		size++;

	return size;
}

BOOL isStringEquals(const char* stringA, const char* stringB)
{
	if (sizeOfString(stringA) != sizeOfString(stringB))
		return FALSE;

	for (int i = 0; stringA[i] != '\0' && stringB[i] != '\0'; i++)
	{
		if (stringA[i] != stringB[i])
			return FALSE;
	}

	return TRUE;
}