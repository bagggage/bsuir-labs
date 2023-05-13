#include "cache.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#define DNS_DB_MAX_STRING_SIZE 256

typedef enum DomainType 
{
	DT_RAW,
	DT_REFERENCE

} DomainType;

const size_t getDnsCacheElementSize() 
{
	return sizeof(QueueElement) + sizeof(Domain) + sizeof(Pair) + DOMAIN_NAME_AVARAGE_LENGTH;
}

DNSCache initDnsCache(size_t capacity, const char* databasePath) 
{
	assert(capacity > 0);

	DNSCache cache =
	{
		.capacity = capacity,
		.hashTable = initHashTable(capacity),
		.queue = initQueue(),
		.databasePath = NULL
	};

	if (databasePath != NULL)
	{
		if (fillDnsCacheFromFile(&cache, databasePath) == false)
		{
			printf("Loading database from file \"%s\" failed\n", databasePath);

			return cache;
		}

		unsigned int pathLength = strnlen_s(databasePath, DNS_DB_MAX_STRING_SIZE);

		cache.databasePath = (char*)malloc(pathLength + 1);

		if (cache.databasePath == NULL)
		{
			perror("Can't save database path to cache, memory allocation failed");

			return cache;
		}

		strcpy_s(cache.databasePath, pathLength + 1, databasePath);
	}

	return cache;
}

void destructDnsCache(DNSCache* dnsCache)
{
	assert(dnsCache != NULL);

	destructHashTable(&dnsCache->hashTable);
	destructQueue(&dnsCache->queue);

	if (dnsCache->databasePath != NULL)
		free(dnsCache->databasePath);

	dnsCache->capacity = 0;
	dnsCache->databasePath = NULL;
}

void skipEmptySymbols(char** string) 
{
	assert(string != NULL && *string != NULL);

	while (**string == ' ' || **string == '\t' || **string == '\n')
		++(*string);
}

void skipNonEmptySymbols(char** string) 
{
	assert(string != NULL && *string != NULL);

	while (**string != ' ' && **string != '\t' && **string != '\n' && **string != '\0')
		++(*string);
}

const char* inLexem = "IN";
const char* rawTypeLexem = "A";
const char* referenceTypeLexem = "CNAME";

bool isStringIpCorrect(const char* stringValue) 
{
	unsigned char dotsCount = 0;
	unsigned char numsCount = 0;

	for (int i = 0; stringValue[i] != '\0'; ++i)
		if (isdigit(stringValue[i]))
		{
			++numsCount;
		}
		else if (stringValue[i] == '.' && numsCount > 0 && numsCount < 4)
		{
			numsCount = 0;
			++dotsCount;
		}
		else
		{
			return false;
		}

	return (dotsCount == 3 && numsCount > 0);
}

bool parseIp(char* stringValue, unsigned char ipOut[4]) 
{
	assert(stringValue != NULL);

	if (isStringIpCorrect(stringValue) == false)
		return false;

	for (int i = 0; i < 4; ++i)
	{
		unsigned char g = 0;

		while (isdigit(stringValue[g]))
			++g;

		stringValue[g] = '\0';

		int value = atoi(stringValue);

		if (value > 255)
			return false;

		ipOut[i] = (char)value;

		stringValue += g + 1;
	}

	return true;
}

Domain* parseDomain(char* stringBuffer, char** referenceNameOut);

bool findRawDomainIpInFile(FILE* file, const char* domainName, unsigned char ipOut[4])
{
	assert(file != NULL);
	assert(domainName != NULL);

	char* stringBuffer = (char*)calloc(DNS_DB_MAX_STRING_SIZE, sizeof(char));

	if (stringBuffer == NULL)
	{
		perror("Can't find raw domain ip, memory allocation failed");
		return false;
	}

	while (fgets(stringBuffer, DNS_DB_MAX_STRING_SIZE, file) != NULL) 
	{
		char* temp = stringBuffer;

		skipEmptySymbols(&temp);

		const char* currentDomainName = temp;

		//Domain Name
		{
			skipNonEmptySymbols(&temp);

			if (*temp == '\0')
				continue;

			*temp = '\0';
			++temp;
		}

		if (strcmp(domainName, currentDomainName) != 0)
			continue;

		char* referenceNameOut;

		*(--temp) = ' ';

		Domain* domain = parseDomain(stringBuffer, &referenceNameOut);

		if (domain == NULL)
		{
			free(stringBuffer);
			return false;
		}

		if (referenceNameOut != NULL)
		{
			destructDomain(domain);
			free(domain);

			bool result = findRawDomainIpInFile(file, referenceNameOut, ipOut);

			free(stringBuffer);

			return result;
		}

		*(unsigned int*)ipOut = *(unsigned int*)domain->ip;

		destructDomain(domain);
		free(stringBuffer);
		free(domain);

		return true;
	}

	free(stringBuffer);

	return false;
}

Domain* parseDomain(char* stringBuffer, char** referenceNameOut) 
{
	assert(stringBuffer != NULL && referenceNameOut != NULL);

	char* temp = stringBuffer;

	skipEmptySymbols(&temp);

	const char* domainName = temp;

	//Domain Name
	{
		skipNonEmptySymbols(&temp);

		if (*temp == '\0')
			return NULL;

		*temp = '\0';
		++temp;

		skipEmptySymbols(&temp);
	}

	//IN - Lexem
	{
		char* lexem = temp;

		skipNonEmptySymbols(&temp);

		if (*temp == '\0')
			return NULL;

		*temp = '\0';
		++temp;

		if (strcmp(lexem, inLexem) != 0)
		{
			printf("Unknown symbol \"%s\" in domain defenition \"%s\"\n", lexem, domainName);
			return NULL;
		}

		skipEmptySymbols(&temp);
	}

	DomainType domainType;

	//Domain Type
	{
		const char* type = temp;

		skipNonEmptySymbols(&temp);

		if (*temp == '\0')
			return NULL;

		*temp = '\0';
		++temp;

		if (strcmp(type, rawTypeLexem) == 0)
		{
			domainType = DT_RAW;
		}
		else if (strcmp(type, referenceTypeLexem) == 0)
		{
			domainType = DT_REFERENCE;
		}
		else
		{
			printf("Unknown type \"%s\" for domain \"%s\"\n", type, domainName);
			return NULL;
		}

		skipEmptySymbols(&temp);
	}

	char* domainValue = temp;

	//Domain Value
	{
		skipNonEmptySymbols(&temp);

		*temp = '\0';
		++temp;
	}

	Domain* domain = (Domain*)malloc(sizeof(Domain));

	if (domain == NULL)
	{
		perror("Can't parse domain, memory allocation failed");
		return NULL;
	}

	unsigned char ip[4] = { 0 };

	*domain = makeDomain(domainName, ip);

	if (domain->name == NULL)
	{
		free(domain);
		return NULL;
	}

	switch (domainType)
	{
	case DT_RAW:
		*referenceNameOut = NULL;

		if (parseIp(domainValue, domain->ip) == false)
		{
			printf("Incorrect ip value \"%s\", for domain \"%s\"\n", domainValue, domainName);
			destructDomain(domain);
			free(domain);

			return NULL;
		}
		break;
	case DT_REFERENCE:
		*referenceNameOut = domainValue;
		break;
	default:
		assert(false);
		break;
	}

	return domain;
}

bool fillDnsCacheFromFile(DNSCache* cache, const char* filePath)
{
	assert(cache != NULL);
	assert(filePath != NULL);

	FILE* file;

	errno_t openResult = fopen_s(&file, filePath, "r");

	if (openResult != 0)
	{
		perror("Can't open dns database");
		return false;
	}

	char* stringBuffer = (char*)calloc(DNS_DB_MAX_STRING_SIZE, sizeof(char));

	if (stringBuffer == NULL)
	{
		perror("Can't load dns database, memory allocation failed");
		fclose(file);

		return false;
	}

	int i = 0;

	while (i < cache->capacity && fgets(stringBuffer, DNS_DB_MAX_STRING_SIZE, file) != NULL)
	{
		char* referenceDomainName;

		Domain* domain = parseDomain(stringBuffer, &referenceDomainName);

		if (domain == NULL)
			continue;

		if (referenceDomainName != NULL)
		{
			long currentFilePos = ftell(file);

			fseek(file, 0, SEEK_SET);

			if (findRawDomainIpInFile(file, referenceDomainName, domain->ip) == false)
			{
				printf("Ip address for domain \"%s\", that refers to \"%s\", not found\n", domain->name, referenceDomainName);

				destructDomain(domain);
				free(domain);

				fseek(file, currentFilePos, SEEK_SET);

				continue;
			}

			fseek(file, currentFilePos, SEEK_SET);
		}

		QueueElement* repeating;

		if ((repeating = (QueueElement*)hashTableGet(&cache->hashTable, domain->name)) != NULL)
		{
			if (*(unsigned int*)repeating->value->ip != *(unsigned int*)domain->ip)
				printf("Redefenition! Domain \"%s\" already loaded into cache, override skipped\n", domain->name);
			else
				printf("Repeating! Domain \"%s\" already loaded into cache\n", domain->name);

			destructDomain(domain);
			free(domain);

			continue;
		}

		if (enQueue(&cache->queue, domain) == false)
		{
			destructDomain(domain);
			free(domain);

			continue;
		}

		if (hashTableSet(&cache->hashTable, domain->name, cache->queue.head) == false) 
		{
			deQueueHead(&cache->queue);

			continue;
		}

		printf("Domain loaded into cache - ");
		printDomain(domain);

		++i;
	}

	free(stringBuffer);

	if (ferror(file) != 0)
	{
		perror("Loading dns database failed while reading database file");
		fclose(file);

		return false;
	}

	fclose(file);

	return true;
}

bool addLastDomainInFile(DNSCache* cache) 
{
	assert(cache != NULL);
	assert(cache->queue.head != NULL);

	FILE* file;

	errno_t openResult = fopen_s(&file, cache->databasePath, "a+");

	if (openResult != 0) 
	{
		perror("Can't open dns database");
		return false;
	}

	Domain* lastDomain = cache->queue.head->value;

	fseek(file, -1, SEEK_END);

	if (fgetc(file) != '\n')
		fputc('\n', file);

	int writeResult = fprintf_s
	(
		file, "%s\tIN\tA\t%u.%u.%u.%u\n", 
		lastDomain->name,
		(unsigned int)lastDomain->ip[0],
		(unsigned int)lastDomain->ip[1],
		(unsigned int)lastDomain->ip[2],
		(unsigned int)lastDomain->ip[3]
	);

	fclose(file);

	if (writeResult < 0)
	{
		perror("Writing domain in file failed");
		return false;
	}

	return true;
}

bool addDnsDomain(DNSCache* cache, const char* name, const char* ipString) 
{
	assert(cache != NULL);
	assert(name != NULL);
	assert(ipString != NULL);

	if (getDnsDomainByName(cache, name) != NULL)
	{
		printf("Domain with name \"%s\" already exists\n", name);
		return false;
	}

	unsigned char ipStringLength = strnlen_s(ipString, UCHAR_MAX);
	const char* ipStringCopy = malloc(ipStringLength + 1);

	if (ipStringCopy == NULL)
	{
		perror("Can't add domain, memory allocation failed");
		return false;
	}

	strcpy_s(ipStringCopy, ipStringLength + 1, ipString);

	unsigned char ip[4];

	if (parseIp(ipStringCopy, ip) == false)
	{
		free(ipStringCopy);
		return false;
	}

	free(ipStringCopy);

	Domain* domain = (Domain*)malloc(sizeof(Domain));

	if (domain == NULL) 
	{
		perror("Can't create domain, memory allocation failed");
		return false;
	}

	*domain = makeDomain(name, ip);

	if (domain->name == NULL)
	{
		free(domain);
		return false;
	}

	if (cache->queue.size >= cache->capacity)
	{
		hashTableDelete(&cache->hashTable, cache->queue.tail->value->name);
		deQueue(&cache->queue);
	}

	if (enQueue(&cache->queue, domain) == false)
	{
		destructDomain(domain);
		free(domain);

		return false;
	}

	if (hashTableSet(&cache->hashTable, domain->name, cache->queue.head) == false) 
	{
		deQueueHead(&cache->queue);

		return false;
	}

	if (cache->databasePath != NULL)
		return addLastDomainInFile(cache);

	return true;
}

Domain* getDnsDomainFromFile(DNSCache* cache, const char* name) 
{
	assert(cache != NULL);
	assert(cache->databasePath != NULL);
	assert(name != NULL);
	assert(*name != '\0');

	FILE* file;

	errno_t openResult = fopen_s(&file, cache->databasePath, "r");

	if (openResult != 0)
	{
		perror("Can't open dns database to load domain");
		return NULL;
	}

	char* stringBuffer = (char*)calloc(DNS_DB_MAX_STRING_SIZE, sizeof(char));

	if (stringBuffer == NULL)
	{
		perror("Can't load dns database, memory allocation failed");
		fclose(file);

		return NULL;
	}

	Domain* domain = NULL;

	while (fgets(stringBuffer, DNS_DB_MAX_STRING_SIZE, file) != NULL)
	{
		char* temp = stringBuffer;

		skipEmptySymbols(&temp);

		const char* domainName = temp;

		//Domain Name
		{
			skipNonEmptySymbols(&temp);

			if (*temp == '\0')
				continue;

			*temp = '\0';
			++temp;
		}

		if (strcmp(name, domainName) != 0)
			continue;

		*(--temp) = ' ';

		char* referenceDomainName;

		domain = parseDomain(stringBuffer, &referenceDomainName);

		if (domain == NULL)
		{
			fclose(file);
			free(stringBuffer);
			return NULL;
		}

		if (referenceDomainName != NULL)
		{
			long currentFilePos = ftell(file);

			fseek(file, 0, SEEK_SET);

			if (findRawDomainIpInFile(file, referenceDomainName, domain->ip) == false)
			{
				destructDomain(domain);
				free(domain);
				free(stringBuffer);
				fclose(file);

				return NULL;
			}

			fseek(file, currentFilePos, SEEK_SET);
		}

		if (cache->queue.size >= cache->capacity)
		{
			hashTableDelete(&cache->hashTable, cache->queue.tail->value->name);
			deQueue(&cache->queue);
		}

		if (enQueue(&cache->queue, domain) == false) 
		{
			destructDomain(domain);
			free(domain);
			free(stringBuffer);
			fclose(file);

			return NULL;
		}

		if (hashTableSet(&cache->hashTable, domain->name, cache->queue.head) == false)
		{
			deQueueHead(&cache->queue);
			free(stringBuffer);
			fclose(file);

			return NULL;
		}
	}

	free(stringBuffer);

	if (ferror(file) != 0)
		perror("Something went wrong while reading dns database");

	fclose(file);

	return domain;
}

Domain* getDnsDomainByName(DNSCache* cache, const char* name) 
{
	assert(cache != NULL);
	assert(name != NULL);

	QueueElement* elementInCache = (QueueElement*)hashTableGet(&cache->hashTable, name);

	if (elementInCache != NULL)
	{
		prioritizeQueueElement(&cache->queue, elementInCache);

		return elementInCache->value;
	}

	if (cache->databasePath != NULL)
		return getDnsDomainFromFile(cache, name);
	else
		return NULL;
}

bool pushDomainInArray(Domain*** array, unsigned int* size, Domain* domain) 
{
	assert(array != NULL);
	assert(size != NULL);

	++(*size);

	Domain** newArrayPtr;

	if (*array == NULL)
		newArrayPtr = malloc(*size * sizeof(Domain*));
	else
		newArrayPtr = realloc(*array, *size * sizeof(Domain*));

	if (newArrayPtr == NULL)
	{
		perror("Can't push domain in array, memory allocation failed");
		--(*size);

		return false;
	}

	*array = newArrayPtr;

	newArrayPtr[(*size) - 1] = domain;

	return true;
}

Domain** findAllDomainsInFileWithIp(unsigned char ip[4], unsigned int* sizeOut, const char* filePath) 
{
	assert(sizeOut != NULL);
	assert(filePath != NULL);

	*sizeOut = 0;

	Domain** array = NULL;

	FILE* file;

	errno_t openResult = fopen_s(&file, filePath, "r");

	if (openResult != 0)
	{
		perror("Can't open dns database to load domain");
		return NULL;
	}

	char* stringBuffer = (char*)calloc(DNS_DB_MAX_STRING_SIZE, sizeof(char));

	if (stringBuffer == NULL)
	{
		perror("Can't load dns database, memory allocation failed");
		fclose(file);

		return NULL;
	}

	while (fgets(stringBuffer, DNS_DB_MAX_STRING_SIZE, file) != NULL) 
	{
		char* referenceDomainName;

		Domain* domain = parseDomain(stringBuffer, &referenceDomainName);

		if (domain == NULL)
			continue;

		if (referenceDomainName != NULL)
		{
			long currentFilePos = ftell(file);

			fseek(file, 0, SEEK_SET);

			if (findRawDomainIpInFile(file, referenceDomainName, domain->ip) == false)
			{
				destructDomain(domain);
				free(domain);

				fseek(file, currentFilePos, SEEK_SET);

				continue;
			}

			fseek(file, currentFilePos, SEEK_SET);
		}

		if (*(unsigned int*)domain->ip == *(unsigned int*)ip)
			if (pushDomainInArray(&array, sizeOut, domain) == false)
			{
				destructDomain(domain);
				free(domain);
				free(stringBuffer);
				fclose(file);

				return array;
			}
	}

	free(stringBuffer);

	if (ferror(file) != 0)
		perror("Something went wrong while reading dns database");

	fclose(file);

	return array;
}

Domain** getDnsDomainsByIp(DNSCache* cache, const char* ipString, unsigned int* sizeOut) 
{
	assert(cache != NULL);
	assert(ipString != NULL);
	assert(sizeOut != NULL);

	*sizeOut = 0;

	unsigned char ipLength = strnlen_s(ipString, UCHAR_MAX);
	char* ipStringCopy = malloc(ipLength + 1);

	if (ipStringCopy == NULL)
	{
		perror("Can't parse ip, memory allocation failed");
		return NULL;
	}

	strcpy_s(ipStringCopy, ipLength + 1, ipString);

	unsigned char ip[4];

	if (parseIp(ipStringCopy, ip) == false)
	{
		printf("Can't find domains by ip address \"%s\", incorrect value\n", ipStringCopy);
		free(ipStringCopy);

		return NULL;
	}

	free(ipStringCopy);

	//Search in cache
	if (cache->databasePath == NULL)
	{
		Domain** array = NULL;
		QueueElement* element = cache->queue.head;

		while (element != NULL) 
		{
			if (*(unsigned int*)element->value->ip == *(unsigned int*)ip)
				if (pushDomainInArray(&array, sizeOut, element->value) == false)
					return array;

			element = element->next;
		}

		return array;
	}

	return findAllDomainsInFileWithIp(ip, sizeOut, cache->databasePath);
}
