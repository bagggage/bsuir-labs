#pragma once

#include <stdlib.h>

#define HASH_TABLE_SIZE 100

typedef struct Pair 
{
	char* key;
	char* value;

	struct Pair* next;
} Pair;

typedef struct Dictionary 
{
	Pair* hashTable[HASH_TABLE_SIZE];
} Dictionary;

Dictionary dictionaryInit();

const char* dictionaryLookUp(Dictionary* dictionary, const char* key);
Pair* dictionaryLookUpPair(Dictionary* dictionary, const char* key);

void dictionaryInsert(Dictionary* dictionary, char* key, char* value);
void dictionaryClear(Dictionary* dictionary);
void dictionaryDestroy(Dictionary* dictionary);
