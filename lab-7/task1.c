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
	printf("%d", number + number2);
}

int main() 
{
	MenuType menu = initMenu();

	addMenuItem(menu, "Count words in string", task1);
	addMenuItem(menu, "Dublicate char in string", task2);

	do
	{
		drawMenu(menu);

	} while (handleMenu(menu));

	destroyMenu(&menu);

	return 0;
}