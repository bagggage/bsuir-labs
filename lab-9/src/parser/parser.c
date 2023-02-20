// Copyright: 2023 Pigulevskiy Konstantin
// Lisence: MIT

#define _CRT_SECURE_NO_DEPRECATE

#include "parser.h"

#include "utils.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#define isCurrentSymbol(c) (context->pageData[context->cursorPos] == c)
#define isCurrentSymbolNot(c) (context->pageData[context->cursorPos] != c)
#define isEndOfSource isCurrentSymbol('\0')
#define isContextSymbolEquals(offset, c) (context->pageData[context->cursorPos + offset] == c)
#define isContextSymbolNotEquals(offset, c) (context->pageData[context->cursorPos + offset] != c)
#define isContextEndOfSource(offset) isContextSymbolEquals(offset, '\0')

#define quotesCheck() if (isCurrentSymbol('\"')) context->quotes += (context->quotes < 1 ? 1 : -1)
#define quotesCheckContext(offset) if (isContextSymbolEquals(offset, '\"')) context->quotes += (context->quotes < 1 ? 1 : -1)

#define error(msg) printf(msg); return FALSE

#pragma region hardcoded-tags
const char* selfClosingTags[] =
{
	"area",
	"base",
	"br",
	"col",
	"embed",
	"hr",
	"img",
	"input",
	"link",
	"meta",
	"param",
	"source",
	"track",
	"wbr"
};

const char* tagsToSkipByDefalut[] =
{
	"head",
	"meta",
	"script",
	"link",
	"noscript"
};
#pragma endregion

Parser* context;

BOOL isInQuotes() 
{
	return context->quotes > 0;
}

BOOL isSelfClosingTag()
{
	for (int i = 0; i < sizeof(selfClosingTags) / sizeof(const char*); i++)
		if (strcmp(context->currentTagName, selfClosingTags[i]) == 0)
			return TRUE;

	return FALSE;
}

BOOL isShouldSkipTagByDefault() 
{
	for (int i = 0; i < sizeof(tagsToSkipByDefalut) / sizeof(const char*); i++)
		if (strcmp(context->currentTagName, tagsToSkipByDefalut[i]) == 0)
			return TRUE;

	return FALSE;
}

BOOL skipEmptySymbols() 
{
	while (isCurrentSymbol(' ') && !isEndOfSource)
		context->cursorPos++;

	return !isEndOfSource;
}

BOOL skipTo(char c) 
{
	while (isCurrentSymbolNot(c) && !isEndOfSource)
		context->cursorPos++;

	return !isEndOfSource;
}

BOOL nextTag() 
{
	while(!isEndOfSource)
	{
		if (isCurrentSymbol('<'))
		{
			context->cursorPos++;

			if (isCurrentSymbol('!') || isCurrentSymbol('/'))
			{
				if (isCurrentSymbol('/'))
				{
					context->openedTags--;
					context->lastClosedChildTreeDeepth = context->openedTags;
				}

				skipTo('>');

				context->cursorPos++;

				continue;
			}

			if (skipEmptySymbols() == FALSE)
				return FALSE;

			unsigned int tagNameOffset = 1;

			while (isContextSymbolNotEquals(tagNameOffset, '>') 
				&& isContextSymbolNotEquals(tagNameOffset, ' ') 
				&& !isContextEndOfSource(tagNameOffset))
				tagNameOffset++;

			if (context->currentTagName != NULL)
				free(context->currentTagName);

			context->currentTagName = makeStringFromRange
			(
				context->pageData, 
				context->cursorPos,
				context->cursorPos + tagNameOffset - 1
			);

			context->cursorPos += tagNameOffset;

			if (isSelfClosingTag() == FALSE)
				context->openedTags++;

			return !isEndOfSource;
		}

		context->cursorPos++;

		if (isCurrentSymbol('\"'))
		{
			context->quotes += (context->quotes < 1 ? 1 : -1);
			context->cursorPos++;
		}
	}

	return FALSE;
}

BOOL skipCurrentTag()
{
	if (isSelfClosingTag())
		return skipTo('>');

	unsigned int currentOpenedTagsCount = context->openedTags;

	context->lastClosedChildTreeDeepth = context->openedTags;

	BOOL isNotEndOfSource;

	do
	{
		isNotEndOfSource = nextTag();

	} while (context->lastClosedChildTreeDeepth >= currentOpenedTagsCount && isNotEndOfSource);

	return isNotEndOfSource;
}

/*Returns: 
-1: end of source
0: identifier is not found
Other positive value: Offset of the identifier end(char after identifier name) to the cursor pos
*/
int findTagIdentifier(const char* identifierName) 
{
	unsigned int identifierStartOffset = 0;

	while (isContextSymbolNotEquals(identifierStartOffset, '>') 
		&& isContextSymbolNotEquals(identifierStartOffset, '/')
		&& !isContextEndOfSource(identifierStartOffset))
	{
		if (isContextSymbolEquals(identifierStartOffset, ' '))
		{
			identifierStartOffset++;
			continue;
		}
		
		unsigned int identifierEndOffset = identifierStartOffset + 1;

		while (isContextSymbolNotEquals(identifierEndOffset, '>')
			&& isContextSymbolNotEquals(identifierEndOffset, '/')
			&& isContextSymbolNotEquals(identifierEndOffset, '=')
			&& isContextSymbolNotEquals(identifierEndOffset, ' ')
			&& !isContextEndOfSource(identifierEndOffset))
				identifierEndOffset++;

		if (isContextEndOfSource(identifierEndOffset))
			return -1;

		char* currentIdentifierName = makeStringFromRange
		(
			context->pageData,
			context->cursorPos + identifierStartOffset,
			context->cursorPos + identifierEndOffset - 1
		);

		if (strcmp(currentIdentifierName, identifierName) == 0)
		{
			free(currentIdentifierName);
			return identifierEndOffset;
		}
		
		free(currentIdentifierName);
		identifierStartOffset = identifierEndOffset;

		if (isContextSymbolEquals(identifierStartOffset, '='))
		{
			do
			{
				identifierStartOffset++;
				quotesCheckContext(identifierStartOffset);

			} while (isInQuotes() && !isContextEndOfSource(identifierStartOffset));

			if (isContextSymbolEquals(identifierStartOffset, '\"'))
				identifierStartOffset++;
		}
	}

	return (isContextEndOfSource(identifierStartOffset) ? -1 : 0);
}

char* getIdentifierValue(int identifierOffset)
{
	if (isContextSymbolNotEquals(identifierOffset, '='))
		return NULL;

	int identifierValueOffset = identifierOffset + 1;

	do
	{
		identifierValueOffset++;

		if (isContextSymbolEquals(identifierValueOffset, '\"'))
			break;

	} while (!isContextEndOfSource(identifierValueOffset));

	if (isContextEndOfSource(identifierValueOffset))
		return NULL;

	char* identifierValue = makeStringFromRange
	(
		context->pageData,
		context->cursorPos + identifierOffset + 2,
		context->cursorPos + identifierValueOffset - 1
	);

	return identifierValue;
}

/*Returns:
-1: end of source
0: value is not equals
1: value is equals
*/
int isIdentifierValueEquals(int identifierOffset, const char* value, BOOL isStrongCompare)
{
	char* currentIdentifierValue = getIdentifierValue(identifierOffset);

	if (currentIdentifierValue == NULL)
		return 0;

	BOOL result;

	if (isStrongCompare)
		result = (strcmp(currentIdentifierValue, value) == 0);
	else
		result = softStringCompareOnEquality(currentIdentifierValue, value);

	free(currentIdentifierValue);

	return result;
}

/*Returns:
-1: end of source
0: value is not equals
1: value is equals
*/
int findAndCompareIdentifier(const char* identifierName, const char* value, BOOL isStrongCompare) 
{
	int offset = findTagIdentifier(identifierName);

	if (offset < 1)
		return offset;

	if (value != NULL)
		return isIdentifierValueEquals(offset, value, isStrongCompare);
	else
		return TRUE;
}

int isCurrentTagDesired(const TagMeta* desiredTag) 
{
	if (strcmp(context->currentTagName, desiredTag->name) != 0)
		return 0;

	if (desiredTag->class != NULL)
	{
		int result = findAndCompareIdentifier("class", desiredTag->class, desiredTag->isClassStrongCompare);

		if (result < 1)
			return result;
	}

	if (desiredTag->specificIdentifier != NULL)
	{
		int result = findAndCompareIdentifier(desiredTag->specificIdentifier, desiredTag->specificIdentifierValue, desiredTag->isIdentifierStrongCompare);

		if (result < 1)
			return result;
	}

	if (desiredTag->isShouldGetDataFromIdentifiers && desiredTag->dataIdentifiersCount > 0) 
	{
		for (int i = 0; i < desiredTag->dataIdentifiersCount; i++)
		{
			int result = findTagIdentifier(desiredTag->dataIdentifiers[i]);

			if (result < 1)
				return result;
		}
	}

	return 1;
}

const TagMeta* searchForTags(const TagMeta* tags, unsigned int count) 
{
	assert(tags != NULL);

	if (count < 1)
		return NULL;

	BOOL isNotEndOfSource;

	while ((isNotEndOfSource = nextTag()))
	{
		if (isShouldSkipTagByDefault())
		{
			if (skipCurrentTag())
				continue;
			else
				return NULL;
		}

		for (int i = 0; i < count; i++)
		{
			int result = isCurrentTagDesired(&tags[i]);

			switch (result)
			{
			case -1:
				return NULL;
			case 0:
				continue;
			case 1:
				return &tags[i];
			default:
				assert(FALSE);
			}
		}
	}

	return isNotEndOfSource;
}

BOOL findTag(const TagMeta* tag) 
{
	assert(tag != NULL);
	assert(tag->name != NULL);

	return (searchForTags(tag, 1) != NULL);
}

BOOL parseCurrentTag(ParsedData* parsedData, const TagMeta* meta) 
{
	assert(parsedData != NULL);
	assert(meta != NULL);

	if (meta->isShouldGetDataFromIdentifiers) 
	{
		for (int i = 0; i < meta->dataIdentifiersCount; i++)
		{
			int identifierOffset = findTagIdentifier(meta->dataIdentifiers[i]);
			char* value = getIdentifierValue(identifierOffset);

			assert(identifierOffset > 0);

			if (value == NULL)
				value = initEmptyString();
			else if (context->setup->isShouldSaveRaw == FALSE)
				removeUnnessesarySymbols(&value);

			pushParsedValueForTag(parsedData, meta, value);
		}
	}
	else 
	{
		if (skipTo('>') == FALSE)
			return FALSE;

		context->cursorPos++;

		int indexOfBeginValue = context->cursorPos;

		while (isCurrentSymbolNot('<') && !isEndOfSource)
			context->cursorPos++;

		if (isEndOfSource)
			return FALSE;

		char* value = makeStringFromRange
		(
			context->pageData,
			indexOfBeginValue,
			context->cursorPos - 1
		);

		if (value == NULL)
			value = initEmptyString();
		else if (context->setup->isShouldSaveRaw == FALSE)
			removeUnnessesarySymbols(&value);

		pushParsedValueForTag(parsedData, meta, value);
	}

	return TRUE;
}

ParsedData parsePage(const Parser* parser, const char* rawPageData) 
{
	assert(parser != NULL);
	assert(parser->setup != NULL);
	assert(parser->currentTagName == NULL);

	ParsedData parsed = initParsedData();

	if (parser->setup->tagsToParse == NULL || parser->setup->tagsCount == 0)
		return parsed;

	context = parser;

	context->pageData = rawPageData;
	context->cursorPos = 0;

	for (int i = 0; i < context->setup->tagsCount; i++)
		pushParsedTagData(&parsed, initParsedTagData(&context->setup->tagsToParse[i]));

	if (context->setup->targetTag.name != NULL)
	{
		if (findTag(&context->setup->targetTag) == FALSE)
			return parsed;
	}

	unsigned int parentTagChildTreeState = context->openedTags;
	context->lastClosedChildTreeDeepth = context->openedTags;

	while (context->lastClosedChildTreeDeepth >= parentTagChildTreeState && !isEndOfSource)
	{
		const TagMeta* findedTag = searchForTags(context->setup->tagsToParse, context->setup->tagsCount);

		if (findedTag == NULL)
			break;
		
		if (parseCurrentTag(&parsed, findedTag) == FALSE)
			break;
	}

	return parsed;
}

ParseSetup makeSetup() 
{
	ParseSetup setup =
	{
		.isShouldSaveRaw = FALSE,
		.tagsCount = 0,
		.tagsToParse = NULL,
		.targetTag = NULL
	};

	return setup;
}

Parser initParser()
{
	Parser parser =
	{
		.currentTagName = NULL,
		.cursorPos = 0,
		.openedTags = 0,
		.lastClosedChildTreeDeepth = 0,
		.pageData = NULL,
		.quotes = 0,
		.setup = NULL
	};

	return parser;
}

void addTagToParse(ParseSetup* setup, const TagMeta* tag)
{
	assert(setup != NULL);
	assert(tag != NULL);
	assert(tag->name != NULL);

	setup->tagsCount++;

	if (setup->tagsToParse == NULL)
		setup->tagsToParse = malloc(setup->tagsCount * sizeof(TagMeta));
	else
		setup->tagsToParse = realloc(setup->tagsToParse, setup->tagsCount * sizeof(TagMeta));

	if (setup->tagsToParse == NULL)
		return;

	setup->tagsToParse[setup->tagsCount - 1] = *tag;
}

void destroyParsedData(ParsedData* data) 
{
	assert(data != NULL);

	for (int i = 0; i < data->size; i++)
	{
		for (int valIndex = 0; valIndex < data->data[i].size; valIndex++)
			free(data->data[i].values[valIndex]);

		free(data->data[i].values);

		data->data[i].size = 0;
	}

	free(data->data);

	data->size = 0;
}

void destroyParseSetup(ParseSetup* setup) 
{
	assert(setup != NULL);

	if (setup->tagsToParse != NULL)
		free(setup->tagsToParse);

	setup->tagsCount = 0;
}

ParsedTagData getDataForTag(const ParsedData* data, const TagMeta* tag) 
{
	assert(data != NULL);
	assert(tag != NULL);

	for (int i = 0; i < data->size; i++)
		if (isMetaEquals(data->data[i].tag, tag))
			return data->data[i];

	return initParsedTagData(tag);
}
