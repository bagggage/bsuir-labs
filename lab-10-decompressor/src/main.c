#include <stdio.h>
#include <string.h>
#include <exception.h>

#include "decompression.h"

#define MAX_FILE_PATH 256

#define EXTENTION ".decompressed.txt"

int main(int argc, const char** argv)
{
	if (argc < 1)
		throwException(3, "You should enter file path to compress");

	char sourceFilePath[MAX_FILE_PATH] = { '\0' };
	char decompressedFilePath[MAX_FILE_PATH] = { '\0' };

	strcpy_s(sourceFilePath, MAX_FILE_PATH, argv[1]);
	strcpy_s(decompressedFilePath, MAX_FILE_PATH, argv[1]);
	strcat_s(decompressedFilePath, MAX_FILE_PATH, EXTENTION);

	FILE* file;

	int result = fopen_s(&file, sourceFilePath, "r");

	if (result != 0)
	{
		perror("File open error");
		return 1;
	}

	decompress(file, decompressedFilePath);

	fclose(file);

	return 0;
}
