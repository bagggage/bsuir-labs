/*
* Copyright (c) 2023 Pigulevskiy Konstantin
* 
* Permission is hereby granted, free of charge, to any person
* obtaining a copy of this software and associated documentation
* files (the "Software"), to deal in the Software without
* restriction, including without limitation the rights to use,
* copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following
* conditions:
* 
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
* HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
 */
#pragma once

#include <string.h>

#include "tag-meta.h"

typedef struct ParseSetup
{
	/*Tag that consist tags needed to parse, in other words it's a parent tag.*/
	TagMeta targetTag;
	TagMeta* tagsToParse;

	size_t tagsCount;

	/*If true saves data as in the source, otherwise removes spaces at the begining and at end of getted data.*/
	BOOL isShouldSaveRaw;

} ParseSetup;

typedef struct Parser
{
	const char* pageData;
	size_t cursorPos;

	ParseSetup* setup;

	const char* currentTagName;

	unsigned int openedTags;
	unsigned int lastClosedChildTreeDeepth;
	unsigned int quotes;

} Parser;

typedef struct ParsedTagData
{
	TagMeta* tag;

	char** values;
	size_t size;

} ParsedTagData;

/*Struct consists all values parsed from all tags included in ParseSetup.
Don't operate directly.
Use special functions to get needed values from different tags.
Delete it using special free function after work is done.*/
typedef struct ParsedData
{
	ParsedTagData* data;
	size_t size;

} ParsedData;

ParsedData parsePage(Parser* parser, const char* rawPageData);

ParseSetup makeSetup();
Parser initParser();

void addTagToParse(ParseSetup* setup, TagMeta* tag);
void destroyParsedData(ParsedData* data);