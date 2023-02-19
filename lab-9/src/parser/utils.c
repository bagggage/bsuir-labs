// Copyright: 2023 Pigulevskiy Konstantin
// Lisence: MIT

#include "utils.h"

#include <assert.h>
#include <stdlib.h>

char* makeStringFromRange(const char* source, unsigned int firstIndex, unsigned int lastIndex)
{
	if (firstIndex > lastIndex)
		return NULL;

	unsigned int length = lastIndex - firstIndex + 1;

	char* string = malloc((length + 1) * sizeof(char));

	for (int i = 0; i < length; i++)
		string[i] = source[firstIndex + i];

	string[length] = '\0';

	return string;
}

char* initEmptyString() 
{
	char* string = malloc(1 * sizeof(char));

	string[0] = '\0';

	return string;
}

BOOL softStringCompareOnEquality(const char* lhsString, const char* rhsString)
{
	for (int i = 0; lhsString[i] != '\0' && rhsString[i] != '\0'; i++)
		if (lhsString[i] != rhsString[i])
			return FALSE;

	return TRUE;
}

BOOL isMetaEquals(const TagMeta* lhs, const TagMeta* rhs) 
{
	return (
		lhs->name == rhs->name &&
		lhs->class == rhs->class &&
		lhs->specificIdentifier == rhs->specificIdentifier &&
		lhs->specificIdentifierValue == rhs->specificIdentifierValue &&
		lhs->dataIdentifiers == rhs->dataIdentifiers &&
		lhs->dataIdentifiersCount == rhs->dataIdentifiersCount &&
		lhs->isClassStrongCompare == rhs->isClassStrongCompare &&
		lhs->isIdentifierStrongCompare == rhs->isIdentifierStrongCompare &&
		lhs->isShouldGetDataFromIdentifiers == rhs->isShouldGetDataFromIdentifiers &&
		lhs->isShouldGetFirstDeeperData == rhs->isShouldGetFirstDeeperData
		);
}

void removeCharsInString(char** string, int first, int last) 
{
	int stringLength = strlen(*string);
	int step = last - first + 1;

	for (int i = first; i <= stringLength - step; i++)
		(*string)[i] = (*string)[i + step];

	(*string) = realloc(*string, stringLength - step + 1);

	if (*string == NULL)
		exit(1);
}

void removeUnnessesarySymbols(char** string) 
{
	size_t i = 0;

	while ((*string)[i] == ' ')
		i++;

	if (i > 0)
		removeCharsInString(string, 0, i - 1);

	i = strlen(*string) - 1;

	int offset = 0;

	while (i + offset >= 0 && (*string)[i + offset] == ' ')
		offset--;

	if (offset < 0)
		removeCharsInString(string, i - offset + 1, i);
}

void pushStringInArray(char*** array, const char* string, size_t* arraySize) 
{
	assert(array != NULL);
	assert(string != NULL);
	assert(arraySize != NULL);

	(*arraySize)++;

	if (*array == NULL)
		*array = malloc(*arraySize * sizeof(char*));
	else
		*array = realloc(*array, *arraySize * sizeof(char*));

	if (*array == NULL)
		return;

	(*array)[*arraySize - 1] = string;
}

void pushParsedTagData(ParsedData* parsedData, ParsedTagData tagData) 
{
	assert(parsedData != NULL);

	parsedData->size++;

	if (parsedData->data == NULL)
		parsedData->data = malloc(parsedData->size * sizeof(ParsedTagData));
	else
		parsedData->data = realloc(parsedData->data, parsedData->size * sizeof(ParsedTagData));

	if (parsedData->data == NULL)
		return;

	parsedData->data[parsedData->size - 1] = tagData;
}

void pushParsedValueForTag(ParsedData* parsedData, TagMeta* tag, const char* value) 
{
	assert(parsedData != NULL);
	assert(tag != NULL);
	
	for (int i = 0; i < parsedData->size; i++) 
		if (isMetaEquals(parsedData->data[i].tag, tag)) 
		{
			pushStringInArray(&parsedData->data[i].values, value, &parsedData->data[i].size);
			return;
		}

	assert(FALSE);
}

ParsedData initParsedData()
{
	ParsedData parsedData =
	{
		.data = NULL,
		.size = 0
	};

	return parsedData;
}

ParsedTagData initParsedTagData(TagMeta* tag) 
{
	ParsedTagData parsedTagData =
	{
		.tag = tag,
		.values = NULL,
		.size = 0
	};

	return parsedTagData;
}
