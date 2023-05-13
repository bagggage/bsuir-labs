#include "menu-console-handler.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define separator() printf("#---------------#\n");

bool addSubmenu(Menu* menu, const char* name, const Menu* submenu)
{
	assert(menu != NULL);
	assert(submenu != NULL);

	return addMenuItem(menu, name, funcBind(&handleMenuInteraction, 1, submenu));
}

void drawMenu(const Menu* menu)
{
	assert(menu != NULL);

	if (menu->menuItems == NULL)
		return;

	separator();

	for (int i = 0; i < menu->count; i++)
		printf("| %d. %s\n", i + 1, menu->menuItems[i].name);

	separator();

	printf("| 0. %s\n", menu->isSubmenu ? "Back" : "Exit");

	separator();
}

int handleMenuInteraction(const Menu* menu) 
{
	drawMenu(menu);
	int itemNumber = handleMenuInput(menu);

	if (itemNumber == 0)
		return 1;

	if (menu->menuItems[itemNumber - 1].function.functionPtr != NULL)
	{
		system("cls");
		executeFunction(&menu->menuItems[itemNumber - 1].function);
	}
	else
	{
		system("cls");

		drawMenu(menu);

		printf("\nMenu item hasn't implementation");
	}

	return 0;
}

int handleMenuInput(const Menu* menu)
{
	assert(menu != NULL);

	printf("\nSelect menu item: ");

	int menuItemNumber;

	while (scanf_s("%d", &menuItemNumber) < 1 || getchar() != '\n' || menuItemNumber < 0 || menuItemNumber > menu->count)
	{
		system("cls");

		drawMenu(menu);

		printf("\nInvalid item entered. Please select an existing: ");

		rewind(stdin);
	}

	return menuItemNumber;
}