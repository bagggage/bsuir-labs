#include "decompression.h"

#include "algorithm.h"

void decompress(FILE* file, const char* decompressedFileDirectory)
{
	Dictionary dictionary = dictionaryInit();

	fileReadDictionary(&dictionary, file);

	FILE* decompressedFile;

	if (fopen_s(&decompressedFile, decompressedFileDirectory, "w+") != 0)
	{
		perror("File open error");
		return;
	}

	fillFileAndReplaceWords(&dictionary, file, decompressedFile);

	fclose(decompressedFile);
}
