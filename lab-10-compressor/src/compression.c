#include "compression.h"

#include <compression/algorithm.h>

//void printDictionary(const Dictionary* dictionary) 
//{
//	for (int i = 0; i < HASH_TABLE_SIZE; i++)
//	{
//		Pair* pair = dictionary->hashTable[i];
//
//		while (pair != NULL)
//		{
//			printf("Key: %s,\tValue: %s\n", pair->key, pair->value);
//			pair = pair->next;
//		}
//	}
//}

void compress(FILE* file, const char* compressedFileDirectory)
{
	Dictionary dictionary = dictionaryInit();

	makeReplacementDictionary(&dictionary, file);

	FILE* compressedFile;

	if (fopen_s(&compressedFile, compressedFileDirectory, "w+") != 0)
	{
		perror("File open error");
		return;
	}

	fseek(file, 0, SEEK_SET);

	fileWriteDictionary(&dictionary, compressedFile);
	fillFileAndReplaceWords(&dictionary, file, compressedFile);

	fclose(compressedFile);

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
