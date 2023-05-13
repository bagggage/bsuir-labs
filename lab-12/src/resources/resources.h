#pragma once

#include <stdio.h>
#include <stdbool.h>

#define MAX_EXTENTION 16

typedef struct FileName 
{
	char* name;
	char* path;

} FileName;

typedef struct Files 
{
	FileName* data;
	size_t count;

} Files;

FileName initFileName();
Files initFiles();

void destroyFiles(Files** files);

const char* getDataFolderPath();

FILE* openFile(const char* filePath, const char* mode);

char* makeSearchPath(const char* path, const char* extention);
char* makeSaveFilePath(const char* fileName, const char* extention);

Files getListOfFilesInDirectory(const char* path, const char* extention);

