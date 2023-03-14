#pragma once

#include <stdio.h>

#include "bool.h"
#include "../stack/stack.h"
#include "../dictionary/dictionary.h"

void makeReplacementDictionary(Dictionary* dictionary, FILE* file);
void fileReadDictionary(Dictionary* dictionary, FILE* file);
void fileWriteDictionary(Dictionary* dictionary, FILE* file);
void fillFileAndReplaceWords(Dictionary* dictionary, FILE* source, FILE* file);
