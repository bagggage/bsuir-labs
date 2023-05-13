#include "application.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "resources/resources.h"
#include "binary-tree/serialization.h"

Application* currentApplication = NULL;

Application* initApplication() 
{
	assert(currentApplication == NULL);

	currentApplication = calloc(1, sizeof(Application));

	if (currentApplication == NULL) 
	{
		perror("Application initialization failed");

		return NULL;
	}

	return currentApplication;
}

Application* getApplication() 
{
	assert(currentApplication != NULL);

	return currentApplication;
}

void destroyApplication() 
{
	assert(currentApplication != NULL);

	if (currentApplication->dataBase != NULL)
		destroyBinaryBranch(&currentApplication->dataBase);

	currentApplication = NULL;
}

void loadApplicationDataBaseFromFile(const char* filePath) 
{
	assert(filePath != NULL);

	FILE* file = openFile(filePath, "rb");

	if (file == NULL)
		return;

	BinaryTree* dataBase = deserializeBinaryTree(file);

	fclose(file);

	if (dataBase == NULL)
		return;

	if (currentApplication->dataBase != NULL)
		destroyBinaryBranch(&currentApplication->dataBase);

	currentApplication->dataBase = dataBase;
}
