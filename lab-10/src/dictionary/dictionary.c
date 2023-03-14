#include "dictionary.h"

#include <assert.h>
#include <string.h>

#include "../exception.h"

#define MAX_STRING_LENGTH 1024

Dictionary dictionaryInit()
{
	Dictionary dictionary;
	
	for (int i = 0; i < HASH_TABLE_SIZE; i++)
		dictionary.hashTable[i] = NULL;

	return dictionary;
}

unsigned long hashString(const char* string) 
{
	assert(string != NULL);

	unsigned long hash = 5381;
	char c;

	while ((c = *string++))
		hash = ((hash << 5) + hash) + c;

	return hash;
}

const char* dictionaryLookUp(const Dictionary* dictionary, const char* key)
{
	const Pair* pair = dictionaryLookUpPair(dictionary, key);

	if (pair != NULL)
		return pair->value;
	
	return NULL;
}

Pair* dictionaryLookUpPair(const Dictionary* dictionary, const char* key) 
{
	assert(dictionary != NULL);
	assert(key != NULL);

	unsigned int index = hashString(key) % HASH_TABLE_SIZE;

	Pair* pair = dictionary->hashTable[index];

	while (pair != NULL)
	{
		if (pair->key != NULL && strcmp(pair->key, key) == 0)
			return pair;

		pair = pair->next;
	}

	return NULL;
}

void dictionaryInsert(Dictionary* dictionary, const char* key, const char* value) 
{
	assert(dictionary != NULL);

	Pair* pair = malloc(sizeof(Pair));

	if (pair == NULL)
		throwException(1, "Memory allocation failed, dictionary overflow");

	unsigned int index = hashString(key) % HASH_TABLE_SIZE;

	pair->key = key;
	pair->value = value;
	pair->next = dictionary->hashTable[index];

	dictionary->hashTable[index] = pair;
}

void deletePairFromHashTable(Pair* pair) 
{
	if (pair->next != NULL)
		deletePairFromHashTable(pair->next);

	free(pair);
}

void dictionaryClear(Dictionary* dictionary) 
{
	assert(dictionary != NULL);

	for (int i = 0; i < HASH_TABLE_SIZE; i++)
		if (dictionary->hashTable[i] != NULL)
		{
			deletePairFromHashTable(dictionary->hashTable[i]);
			dictionary->hashTable[i] = NULL;
		}
}

void dictionaryDestroy(Dictionary* dictionary) 
{
	dictionaryClear(dictionary);
}
