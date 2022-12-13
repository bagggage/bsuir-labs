#include "string.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define bool char

char* initString(const char* text)
{
	int stringSize = stringLength(text);
	char* string = malloc((stringSize + 1) * sizeof(char));

	for (int i = 0; i < stringSize; i++)
		string[i] = text[i];

	string[stringSize] = '\0';

	return string;
}

int stringLength(const char* text) 
{
	int length = 0;

	while (text[length] != '\0')
		length++;

	return length;
}

bool isDigit(char c) 
{
	if (c > 47 && c < 58)
		return 1;
	else
		return 0;
}

bool isLetter(char c) 
{
	if (!isDigit(c) && ((c > 32 && c < 127) || (c < -8 && c > -33) || c < -80))
		return 1;
	else
		return 0;
}

int countWordsInString(const char* string) 
{
	int countOfWords = 0;

	for (int i = 0; string[i] != '\0'; i++) 
	{
		if (isLetter(string[i]) && (string[i + 1] == ' ' || string[i + 1] == '\n' || string[i + 1] == '\0'))
			countOfWords++;
	}

	return countOfWords;
}