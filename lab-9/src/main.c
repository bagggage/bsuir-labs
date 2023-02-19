#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser/parser.h"
#include "videocard.h"

#pragma region Utils
void fatalError(int exitCode, const char* msg)
{
	printf("\nFatal error: %s\n", msg);
	exit(exitCode);
}

char* duplicateString(const char* source)
{
	size_t length = strlen(source);
	char* duplicate = malloc(length + 1);

	strcpy(duplicate, source);

	return duplicate;
}
#pragma endregion

char* loadDataFromFile(const char* filePath)
{
	char* data;
	FILE* file;

	fopen_s(&file, filePath, "r");

	if (file == NULL)
		fatalError(1, "Cannot open file");

	fseek(file, 0, SEEK_END);

	long length = ftell(file);

	fseek(file, 0, SEEK_SET);

	data = (char*)malloc(length);

	if (data)
		fread(data, 1, length, file);
	else
		fatalError(2, "Memory allocation failed");

	fclose(file);

	data[length] = '\0';

	return data;
}

void nextVendorIndex(unsigned int* currentIndex, ParsedTagData itemSpecifics) 
{
	for ((*currentIndex)++; (*currentIndex) < itemSpecifics.size; (*currentIndex)++)
		if (strcmp(itemSpecifics.values[*currentIndex], "NVIDIA") == 0 || strcmp(itemSpecifics.values[*currentIndex], "AMD") == 0)
			return;

	(*currentIndex)--;
}

Videocard* makeVideocardArrayFromParsedData(ParsedTagData itemInfo, ParsedTagData itemSpecifics, size_t* size)
{
	(*size) = (itemInfo.size / itemInfo.tag->dataIdentifiersCount);

	if (*size == 0)
		return NULL;

	Videocard* cards = malloc(sizeof(Videocard) * (*size));

	if (cards == NULL)
		fatalError(2, "Failed to allocate memory for videocard array");

	unsigned int vendorIndex = 0;

	for (int i = 0; i < *size; i++) 
	{
		cards[i].name = duplicateString(itemInfo.values[i * itemInfo.tag->dataIdentifiersCount]);
		cards[i].price = atof(itemInfo.values[(i * itemInfo.tag->dataIdentifiersCount) + 1]);
		cards[i].producer = duplicateString(itemInfo.values[(i * itemInfo.tag->dataIdentifiersCount) + 2]);
		cards[i].vendor = (strcmp("NVIDIA", itemSpecifics.values[vendorIndex]) == 0 ?
			VideocardVendor_NVIDIA :
			VideocardVendor_AMD
			);


		nextVendorIndex(&vendorIndex, itemSpecifics);
	}

	return cards;
}

void parse21VekSearchPageOnVideocards(const char* rawHtmlPageData, Videocard** resultArray, size_t* count)
{
	Parser parser = initParser();
	ParseSetup setup = makeSetup();

	setup.targetTag = makeTag("ul", "b-result");

	TagMeta itemMain = makeTag("span", "g-item-data");
	TagMeta itemSpecifics = makeTag("td", "result__attr_val  cr-result__attr_odd");

	const char* dataIdentifiers[] = { "data-name", "data-price", "data-producer_name" };

	itemMain.isClassStrongCompare = FALSE;
	itemMain.isShouldGetDataFromIdentifiers = TRUE;
	itemMain.dataIdentifiers = dataIdentifiers;
	itemMain.dataIdentifiersCount = sizeof(dataIdentifiers) / sizeof(char*);

	addTagToParse(&setup, &itemSpecifics);
	addTagToParse(&setup, &itemMain);

	parser.setup = &setup;

	ParsedData data = parsePage(&parser, rawHtmlPageData);

	*resultArray = makeVideocardArrayFromParsedData
	(
		getDataForTag(&data, &itemMain),
		getDataForTag(&data, &itemSpecifics),
		count
	);

	destroyParseSetup(&setup);
	destroyParsedData(&data);
}

void setCodePageUTF8() 
{
	system("chcp 65001");
	system("cls");
}

int main()
{
	setCodePageUTF8();

	const char* htmlPage = loadDataFromFile("E:\\Downloads\\catalog.html");

	Videocard* cards;
	size_t count;

	parse21VekSearchPageOnVideocards(htmlPage, &cards, &count);

	for (int i = 0; i < count; i++)
		videocardLogInfo(&cards[i]);

	for (int i = 0; i < count; i++)
	{
		free(cards[i].name);
		free(cards[i].producer);
	}

	free(cards);

	return 0;
}
