#include <stdio.h>

#include "menu.h"
#include "string.h"

void task1() 
{
	printf("Enter string: ");

	char* string = readString();

	printf("Count of words in string: %d\n", countWordsInString(string));

	destroyString(&string);
}

void task2(int number, int number2)
{
	printf("Enter string: ");

	char* string = readString();

	printf("Enter char to dublicate: ");

	char c;

	while(scanf_s("%c", &c, 1) < 1 || c == '\n' || getchar() != '\n')
	{
		printf("Incorrect input, please try enter char again: ");
		rewind(stdin);
	}

	for (int i = 0; string[i] != '\0'; string[i] == c ? i += 2 : i++)
	{
		if (string[i] == c)
			insertCharToString(&string, i + 1, c);
	}

	printf("Edited string: %s\n", string);

	destroyString(&string);
}

int main() 
{
	MenuType menu = initMenu();

	addMenuItem(menu, "Count words in string", &task1);
	addMenuItem(menu, "Dublicate char in string", &task2);

	do
	{
		drawMenu(menu);

	} while (handleMenu(menu));

	destroyMenu(&menu);

	return 0;
}