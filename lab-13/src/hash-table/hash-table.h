#pragma once

#include <stdbool.h>
#include <stdlib.h>

typedef struct Pair
{
	size_t key;
	void* value;

	struct Pair* next;
} Pair;

typedef struct HashTable 
{
	size_t size;

	Pair* data;

} HashTable;

bool hashTableSet(HashTable* hashTable, const char* key, void* value);
void* hashTableGet(const HashTable* hashTable, const char* key);
void hashTableDelete(HashTable* hashTable, const char* key);

HashTable initHashTable(size_t size);

void destructHashTable(HashTable* hashTable);
