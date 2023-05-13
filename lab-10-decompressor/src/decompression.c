#include "decompression.h"

#include <compression/algorithm.h>

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

	for (int i = 0; i < HASH_TABLE_SIZE; i++)
	{
		Pair* pair = dictionary.hashTable[i];

		while (pair != NULL)
		{
			if (pair->key != NULL)
			{
				Pair* valuePair = dictionaryLookUpPair(&dictionary, pair->key);

				valuePair->key = NULL;
				valuePair->value = NULL;

				free(pair->key);
				free(pair->value);
			}

			pair = pair->next;
		}
	}
}
