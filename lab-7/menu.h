#pragma once

#define bool char
#define MenuType void***
#define MenuItem void(*)(void)

#define MENU_SIZE 0
#define MENU_ITEMS_FUNCS 1
#define MENU_ITEMS_NAMES 2

MenuType initMenu();
void destroyMenu(MenuType* menu);

int getMenuSize(MenuType menu);
void executeMenuItem(MenuType menu, unsigned int functionIndex);
void addMenuItem(MenuType menu, const char* name, void (*function)(void));
void drawMenu(MenuType menu);
bool handleMenu(MenuType menu);