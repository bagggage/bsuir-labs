#include "resources.h"

#include <windows.h>
#include <assert.h>

FileName initFileName()
{
	FileName file =
	{
		.name = NULL,
		.path = NULL
	};

	return file;
}

Files initFiles()
{
	Files files =
	{
		.data = NULL,
		.count = 0
	};

	return files;
}

void destroyFiles(Files** filesPtr) 
{
	assert(filesPtr != NULL);
	assert(*filesPtr != NULL);

	free(*filesPtr);

	*filesPtr = NULL;
}

const char* getDataFolderPath()
{
	return "Data\\";
}

FILE* openFile(const char* filePath, const char* mode) 
{
	FILE* file;

	if (fopen_s(&file, filePath, mode) != 0)
		perror("Can't open file");

	return file;
}

FileName getFileNameFromFindData(const WIN32_FIND_DATA* findData, const char* path)
{
	FileName fileName = initFileName();

	unsigned short nameLength = strnlen_s(findData->cFileName, MAX_PATH);

	fileName.name = malloc(nameLength + 1);

	if (fileName.name == NULL) 
	{
		perror("Memory allocation failed");
		return fileName;
	}

	if (strcpy_s(fileName.name, nameLength + 1, findData->cFileName) != 0)
	{
		perror("Can't copy string");
		free(fileName.name);

		fileName.name = NULL;

		return fileName;
	}

	unsigned short pathLength = strnlen_s(path, MAX_PATH) + nameLength;

	fileName.path = malloc(pathLength + 1);

	if (fileName.path == NULL) 
	{
		perror("Memory allocation failed");
		free(fileName.name);

		fileName.name = NULL;

		return fileName;
	}

	if (strcpy_s(fileName.path, pathLength, path) != 0) 
	{
		perror("Can't copy string");
		free(fileName.name);

		fileName.name = NULL;

		return fileName;
	}

	strcpy_s(fileName.path + pathLength - nameLength, nameLength + 1, findData->cFileName);

	return fileName;
}

bool pushFileNameInFiles(Files* files, const FileName* fileName)
{
	FileName* tempData;

	if (files->data == NULL)
		tempData = malloc(sizeof(FileName));
	else
		tempData = realloc(files->data, sizeof(FileName) * (files->count + 1));

	if (tempData == NULL)
		return false;

	files->data = tempData;
	files->count++;

	files->data[files->count - 1] = *fileName;

	return true;
}

char* makeSearchPath(const char* path, const char* extention) 
{
	char* result;

	unsigned short pathLength = strnlen_s(path, MAX_PATH);
	unsigned char extentionLength = strnlen_s(extention, MAX_EXTENTION);
	unsigned short resultLength = pathLength + sizeof(char) + sizeof(char) + extentionLength;

	if (resultLength >= MAX_PATH)
		return NULL;

	result = malloc(resultLength + 1);

	if (result == NULL)
	{
		perror("Memory allocation failed");

		return NULL;
	}

	if (strncpy_s(result, pathLength + 1, path, resultLength) != 0)
	{
		perror("String copy failed");
		free(result);

		return NULL;
	}

	result[pathLength] = '*';
	result[pathLength + 1] = '.';

	char* tempResult = result + pathLength + (sizeof(char) * 2);

	if (strncpy_s(tempResult, extentionLength + 1, extention, extentionLength + 1) != 0)
	{
		perror("String copy failed");
		free(result);

		return NULL;
	}

	return result;
}

char* makeSaveFilePath(const char* fileName, const char* extention)
{
	unsigned short dataPathLength = strnlen_s(getDataFolderPath(), MAX_PATH);
	unsigned short nameLength = strnlen_s(fileName, MAX_PATH);
	unsigned short extentionLength = strnlen_s(extention, MAX_EXTENTION);
	unsigned short length = dataPathLength + nameLength + sizeof(char) + extentionLength;

	char* result = malloc(length + 1);

	if (result == NULL) 
	{
		perror("Memory allocation failed");
		return NULL;
	}

	strncpy_s(result, dataPathLength + 1, getDataFolderPath(), dataPathLength + 1);

	char* temp = result + dataPathLength;

	strncpy_s(temp, nameLength + 1, fileName, nameLength + 1);

	temp += nameLength;

	temp[0] = '.';

	temp += 1;

	strncpy_s(temp, extentionLength + 1, extention, extentionLength + 1);

	return result;
}

Files getListOfFilesInDirectory(const char* path, const char* extention)
{
	assert(path != NULL);
	assert(extention != NULL);

	Files result = initFiles();

	char* searchPath = makeSearchPath(path, extention);

	if (searchPath == NULL)
		return result;

	HANDLE hFind;
	WIN32_FIND_DATA findData;

	hFind = FindFirstFileA(searchPath, &findData);

	bool hasFile = true;

	if (hFind == INVALID_HANDLE_VALUE)
		hasFile = false;

	while (hasFile)
	{
		FileName fileName = getFileNameFromFindData(&findData, path);

		if (fileName.path != NULL)
			if (pushFileNameInFiles(&result, &fileName) == false)
				break;

		hasFile = FindNextFileA(hFind, &findData);
	}

	FindClose(hFind);
	free(searchPath);

	return result;
}
