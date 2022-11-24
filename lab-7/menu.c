#include "menu.h"
#include "string.h"

#include <stdlib.h>
#include <stdio.h>

MenuType initMenu()
{
	void** menu = malloc(sizeof(void*) + sizeof(void**) + sizeof(char**));

	menu[MENU_ITEMS_FUNCS] = malloc(0);
	menu[MENU_ITEMS_NAMES] = malloc(0);
	menu[MENU_SIZE] = 0;

	return menu;
}

void destroyMenu(MenuType* menu) 
{
	for (int i = 0; i < getMenuSize(*menu); i++)
		free((*menu)[MENU_ITEMS_NAMES][i]);

	free((*menu)[MENU_ITEMS_FUNCS]);
	free((*menu)[MENU_ITEMS_NAMES]);
	free(*menu);

	*menu = NULL;
}

int getMenuSize(MenuType menu) 
{
	return ((int)menu[MENU_SIZE]);
}

void executeMenuItem(MenuType menu, int functionIndex) 
{
	((MenuItem)menu[MENU_ITEMS_FUNCS][functionIndex])();
}

void addMenuItem(MenuType menu, const char* name, void (*function)(void)) 
{
	((int)menu[MENU_SIZE]) = ((int)menu[MENU_SIZE]) + 1;

	menu[MENU_ITEMS_FUNCS] = realloc(menu[MENU_ITEMS_FUNCS], (int)(menu[MENU_SIZE]) * sizeof(void*));
	menu[MENU_ITEMS_NAMES] = realloc(menu[MENU_ITEMS_NAMES], (int)(menu[MENU_SIZE]) * sizeof(char*));

	menu[MENU_ITEMS_FUNCS][getMenuSize(menu) - 1] = (void*)function;
	menu[MENU_ITEMS_NAMES][getMenuSize(menu) - 1] = initString(name);
}

void drawMenu(MenuType menu) 
{
	int maxLenghOfItemName = sizeof("0. Exit") - 1;
	int tempLength;

	for (int i = 0; i < getMenuSize(menu); i++)
		if ((tempLength = stringLength(menu[MENU_ITEMS_NAMES][i]) + 3) > maxLenghOfItemName)
			maxLenghOfItemName = tempLength;

	for (int i = 0; i < maxLenghOfItemName; i++)
		printf("-");

	printf("\n");

	for (int i = 0; i < getMenuSize(menu); i++)
		printf("%d. %s\n", i + 1, menu[MENU_ITEMS_NAMES][i]);

	for (int i = 0; i < maxLenghOfItemName; i++)
		printf("-");

	printf("\n0. Exit\n");

	for (int i = 0; i < maxLenghOfItemName; i++)
		printf("-");
}

bool handleMenu(MenuType menu) 
{
	int choise = -1;

	printf("\nChoose option: ");

	while (scanf_s("%d", &choise) < 1 || choise < 0 || choise > getMenuSize(menu) || getchar() != '\n' ) 
	{
		system("cls");

		drawMenu(menu);
		printf("\nIncorrect input, there is no such option, try again: ");

		rewind(stdin);
	}

	if (choise == 0)
	{
		return 0;
	}

	else
	{
		system("cls");
		executeMenuItem(menu, choise - 1);

		printf("\nDo you want continue?\n-----------\n0. Exit\n1. Continue\n-----------\n");
		printf("Choose option: ");

		while (scanf_s("%d", &choise) < 1 || choise < 0 || choise > 1 || getchar() != '\n') 
		{
			system("cls");

			printf("Do you want continue?\n-----------\n0. Exit\n1. Continue\n-----------\n");
			printf("Incorrect input, there is no such option, try again: ");

			rewind(stdin);
		}

		if (choise == 0)
		{
			return 0;
		}

		else
		{
			system("cls");
			return 1;
		}
	}
}