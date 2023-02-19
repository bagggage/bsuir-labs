// Copyright: 2023 Pigulevskiy Konstantin
// Lisence: MIT

#pragma once

#include <string.h>

#include "parser.h"
#include "bool.h"

char* makeStringFromRange(const char* source, unsigned int firstIndex, unsigned int lastIndex);
char* initEmptyString();

BOOL softStringCompareOnEquality(const char* lhsString, const char* rhsString);
BOOL isMetaEquals(const TagMeta* lhs, const TagMeta* rhs);

void removeUnnessesarySymbols(char** string);
void pushStringInArray(char*** array, const char* string, size_t* arraySize);
void pushParsedTagData(ParsedData* parsedData, const ParsedTagData tagData);
void pushParsedValueForTag(ParsedData* parsedData, const TagMeta* tag, const char* value);

ParsedData initParsedData();
ParsedTagData initParsedTagData(const TagMeta* tag);