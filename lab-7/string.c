#include "string.h"

#include <stdlib.h>
#include <stdio.h>

#define bool char

char* initString(const char* text)
{
	int stringSize = stringLength(text);
	char* string = malloc(stringSize + 1);

	for (int i = 0; i < stringSize; i++)
		string[i] = text[i];

	string[stringSize] = '\0';

	return string;
}

void destroyString(char** stringPtr) 
{
	free(*stringPtr);

	*stringPtr = NULL;
}

char* readString() 
{
	char* string = initString("");
	char tempChar;

	while ((tempChar = getchar()) != '\n')
		pushBackCharToString(&string, tempChar);

	return string;
}

int stringLength(char* text) 
{
	int length = 0;

	while (text[length] != '\0')
		length++;

	return length;
}

void pushBackCharToString(char** string, char c) 
{
	int lenght = stringLength(*string) + 1;

	*string = realloc(*string, lenght + 1);

	(*string)[lenght - 1] = c;
	(*string)[lenght] = '\0';
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

int countWordsInString(char* string) 
{
	int countOfWords = 0;

	for (int i = 0; string[i] != '\0'; i++) 
	{
		if (isLetter(string[i]) && (string[i + 1] == ' ' || string[i + 1] == '\n' || string[i + 1] == '\0'))
			countOfWords++;
	}

	return countOfWords;
}