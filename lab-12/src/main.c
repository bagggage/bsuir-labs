#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#include "binary-tree/binary-tree.h"
#include "binary-tree/serialization.h"
#include "game/game.h"
#include "input.h"
#include "editor/binary-tree-editor.h"
#include "resources/resources.h"
#include "console-menu/menu-console-handler.h"
#include "application.h"

void throwErrorAndClearInputBuffer(const char* msg)
{
	printf("%s\n", msg);
	rewind(stdin);
}

void chooseSavedBinaryTreeAndLoadFromFile() 
{
	Files files = getListOfFilesInDirectory(getDataFolderPath(), "bt");

	if (files.count == 0)
	{
		printf("There is no saves\n");

		return;
	}

	Menu dbMenu = initMenu();

	for (int i = 0; i < files.count; i++)
		addMenuItem(&dbMenu, files.data[i].name, funcBind(&loadApplicationDataBaseFromFile, 1, files.data[i].path));

	int result = handleMenuInteraction(&dbMenu);

	if (result == 0 && getApplication()->dataBase == NULL)
		waitForAnyKeyToContinue();

	destroyMenu(&dbMenu);
	destroyFiles(&files);
}

void createNewBinaryTree() 
{
	getApplication()->dataBase = initBinaryNode();
}

void saveCurrentDataBase() 
{
	printf("Enter file name: ");

	char* fileName = readString();

	if (fileName == NULL)
	{
		waitForAnyKeyToContinue();
		return;
	}

	char* filePath = makeSaveFilePath(fileName, "bt");

	if (filePath == NULL)
	{
		waitForAnyKeyToContinue();
		return;
	}

	free(fileName);

	system("cls");
	printf("Saving...\n");

	FILE* fileToSave = openFile(filePath, "wb");

	if (fileToSave == NULL)
	{
		waitForAnyKeyToContinue();
		return;
	}

	if (serializeBinaryTree(getApplication()->dataBase, fileToSave))
		printf("Data base saved succesfully! Path - \"%s\"\n", filePath);
	else
		printf("Something went wrong...\n");

	fclose(fileToSave);
	free(filePath);

	waitForAnyKeyToContinue();
}

int main() 
{
	Application* application = initApplication();

	if (application == NULL)
		return -1;

	Menu loadMenu = initMenu();
	
	addMenuItem(&loadMenu, "Load Saved Data Base", funcBind(&chooseSavedBinaryTreeAndLoadFromFile, 0));
	addMenuItem(&loadMenu, "Create New Data Base", funcBind(&createNewBinaryTree, 0));

	Menu modeMenu = initMenu();

	addMenuItem(&modeMenu, "Start Game", funcBind(&processGame, 0));
	addMenuItem(&modeMenu, "Open Editor", funcBind(&processEditor, 0));
	addMenuItem(&modeMenu, "Save DataBase", funcBind(&saveCurrentDataBase, 0));

	application->menu = &loadMenu;

	while (handleMenuInteraction(application->menu) == 0)
	{
		if (application->dataBase != NULL)
			application->menu = &modeMenu;

		system("cls");
	}

	destroyMenu(&modeMenu);
	destroyMenu(&loadMenu);

	destroyApplication();

	return 0;
}