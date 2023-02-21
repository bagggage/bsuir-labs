#pragma once

#include "../parser/bool.h"

typedef unsigned long long size_t;

typedef void(*FunctionPtr)(void);
typedef void** Args;

typedef struct FunctionBind
{
	FunctionPtr functionPtr;
	Args args;

	unsigned int argsCount;
	
} FunctionBind;

typedef struct MenuItem 
{
	const char* name;
	FunctionBind function;

} MenuItem;

/*Main structure for implementing menu in your application.
Don't create instances directly, use 'init' function.
Clear data using 'destroy' function after work is done.
Also use special functions for setting up and handlind event*/
typedef struct Menu 
{
	MenuItem* menuItems;
	size_t count;

	BOOL isSubmenu;

} Menu;

Menu initMenu();
FunctionBind funcBind(FunctionPtr function, unsigned int argsCount, ...);

void destroyMenu(Menu* menu);
void addMenuItem(Menu* menu, const char* name, FunctionBind function);
void addSubmenu(Menu* menu, const char* name, const Menu* submenu);
void executeFunction(const FunctionBind* function);