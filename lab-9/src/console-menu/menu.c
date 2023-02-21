#include "menu.h"

#include <assert.h>
#include <stdlib.h>
#include <stdarg.h>

Menu initMenu() 
{
	Menu menu =
	{
		.menuItems = NULL,
		.count = 0,
		.isSubmenu = FALSE
	};

	return menu;
}

//FunctionBind supports maximum 4 arguments
FunctionBind funcBind(FunctionPtr function, unsigned int argsCount, ...) 
{
	assert(argsCount <= 4);

	FunctionBind bind =
	{
		.functionPtr = function,
		.args = (argsCount > 0 ? malloc(sizeof(void*) * argsCount) : NULL),
		.argsCount = argsCount
	};

	if (argsCount > 0 && bind.args == NULL)
		exit(2);

	va_list list;
	va_start(list, argsCount);

	for (int i = 0; i < argsCount; i++)
		bind.args[i] = va_arg(list, void*);

	va_end(list);

	return bind;
}

void destroyMenu(Menu* menu) 
{
	assert(menu != NULL);

	for (int i = 0; i < menu->count; i++)
		if (menu->menuItems[i].function.args != NULL)
			free(menu->menuItems[i].function.args);

	free(menu->menuItems);

	menu->menuItems = NULL;
	menu->count = 0;
}

void addMenuItem(Menu* menu, const char* name, FunctionBind function) 
{
	assert(menu != NULL);

	menu->count++;

	if (menu->menuItems == NULL)
		menu->menuItems = malloc(sizeof(MenuItem) * menu->count);
	else
		menu->menuItems = realloc(menu->menuItems, sizeof(MenuItem) * menu->count);

	if (menu->menuItems == NULL)
		exit(2);

	menu->menuItems[menu->count - 1].name = name;
	menu->menuItems[menu->count - 1].function = function;
}

void executeFunction(const FunctionBind* function)
{
	assert(function != NULL);
	assert(function->functionPtr != NULL);

	switch (function->argsCount)
	{
	case 0:
		function->functionPtr();
		break;
	case 1:
		function->functionPtr(function->args[0]);
		break;
	case 2:
		function->functionPtr(function->args[0], function->args[1]);
		break;
	case 3:
		function->functionPtr(function->args[0], function->args[1], function->args[2]);
		break;
	case 4:
		function->functionPtr(function->args[0], function->args[1], function->args[2], function->args[3]);
		break;
	default:
		assert(0);
		break;
	}
}
