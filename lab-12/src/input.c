#include "input.h"

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>

char* readString()
{
	rewind(stdin);

	char* string = malloc(MAX_PATH);

	if (string == NULL)
	{
		perror("Memory allocation error");

		return NULL;
	}

	gets_s(string, MAX_PATH);

	unsigned short stringLength = strnlen_s(string, MAX_PATH);

	char* temp = realloc(string, stringLength + 1);

	if (temp == NULL)
	{
		perror("Memory reallocation failed");
		free(string);

		return NULL;
	}

	return temp;
}

bool readAnswer()
{
	rewind(stdin);

	char answer = '\0';

	while (tolower(answer) != 'y' && tolower(answer) != 'n')
		answer = _getch();

	return (tolower(answer) == 'y');
}

void waitForAnyKeyToContinue() 
{
	printf("\nPress any key to continue...\n");
	
	if (_getch() == 0)
		_getch();
}
