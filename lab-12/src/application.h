#pragma once

#include "binary-tree/binary-tree.h"
#include "console-menu/menu-console-handler.h"

typedef struct Application 
{
	Menu* menu;
	BinaryTree* dataBase;

} Application;

char* makeSavePathForFile(const char* fileName, const char* extention);

Application* initApplication();
Application* getApplication();
void destroyApplication();

void loadApplicationDataBaseFromFile(const char* filePath);
