#include "hash-table.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

unsigned int hashString(const char* string) 
{
	assert(string != NULL);

	unsigned int hash = 5381;
	char c;

	while ((c = *string++))
		hash = ((hash << 5) + hash) + c;

	return hash;
}

Pair makePair(size_t key, const void* value) 
{
	Pair pair =
	{
		.key = key,
		.value = value,
		.next = NULL
	};

	return pair;
}

void destructPair(Pair* pair) 
{
	assert(pair != NULL);

	pair->key = 0;
	pair->value = NULL;
	pair->next = NULL;
}

bool hashTableSet(HashTable* hashTable, const char* key, void* value)
{
	assert(hashTable != NULL);
	assert(hashTable->data != NULL && hashTable->size > 0);
	assert(key != NULL);

	unsigned int hash = hashString(key);
	unsigned int index = hash % hashTable->size;

	Pair* currentPair = &hashTable->data[index];

	if (currentPair->key == 0)
	{
		*currentPair = makePair(hash, value);

		return true;
	}

	while (currentPair->key != hash)
		if (currentPair->next != NULL)
		{
			currentPair = currentPair->next;
		}
		else
		{
			currentPair->next = (Pair*)malloc(sizeof(Pair));

			if (currentPair->next == NULL)
			{
				perror("Can't add new pair in hash table, memory allocation failed");
				return false;
			}

			*currentPair->next = makePair(hash, value);

			return true;
		}

	currentPair->value = value;

	return true;
}

void* hashTableGet(const HashTable* hashTable, const char* key)
{
	assert(hashTable != NULL);
	assert(hashTable->data != NULL && hashTable->size > 0);
	assert(key != NULL);

	unsigned int hash = hashString(key);
	unsigned int index = hash % hashTable->size;

	Pair* currentPair = &hashTable->data[index];

	if (currentPair->key == 0)
		return NULL;

	while (currentPair != NULL)
	{
		if (currentPair->key != hash)
			currentPair = currentPair->next;
		else
			return currentPair->value;
	}

	return NULL;
}

void hashTableDelete(HashTable* hashTable, const char* key) 
{
	assert(hashTable != NULL);
	assert(hashTable->data != NULL && hashTable->size > 0);
	assert(key != NULL);

	unsigned int hash = hashString(key);
	unsigned int index = hash % hashTable->size;

	Pair* currentPair = &hashTable->data[index];

	if (currentPair->key == 0)
		return;

	if (currentPair->key == hash)
	{
		if (currentPair->next != NULL)
		{
			Pair* temp = currentPair->next;
			*currentPair = *temp;

			free(temp);

			return;
		}

		destructPair(currentPair);

		return;
	}

	while (currentPair->next != NULL)
	{
		if (currentPair->next->key != hash)
		{
			currentPair = currentPair->next;
			continue;
		}

		Pair* temp = currentPair->next;

		currentPair->next = temp->next;

		free(temp);

		return;
	}
}

HashTable initHashTable(size_t size) 
{
	assert(size > 0);

	HashTable table =
	{
		.size = size,
		.data = calloc(size, sizeof(Pair))
	};

	if (table.data == NULL)
	{
		perror("Can't create hash table, memory allocation failed");
		table.size = 0;
	}

	return table;
}

void destructHashTable(HashTable* hashTable) 
{
	assert(hashTable != NULL);

	if (hashTable->data != NULL)
	{
		for (size_t i = 0; i < hashTable->size; ++i)
		{
			Pair* pair = hashTable->data[i].next;
			
			while (pair != NULL)
			{
				Pair* temp = pair;

				pair = pair->next;

				free(temp);
			}
		}

		free(hashTable->data);
	}

	hashTable->data = NULL;
	hashTable->size = 0;
}
