#pragma once

#include "../hash-table/hash-table.h"
#include "queue.h"

typedef struct DNSCache
{
	size_t capacity;

	HashTable hashTable;
	PriorityQueue queue;

	char* databasePath;

} DNSCache;

const size_t getDnsCacheElementSize();

DNSCache initDnsCache(size_t capacity, const char* databasePath);

void destructDnsCache(DNSCache* cache);

bool isStringIpCorrect(const char* stringValue);
bool fillDnsCacheFromFile(DNSCache* cache, const char* filePath);

bool addDnsDomain(DNSCache* cache, const char* name, const char* ipString);

Domain* getDnsDomainByName(DNSCache* cache, const char* name);
Domain** getDnsDomainsByIp(DNSCache* cache, const char* ipString, unsigned int* sizeOut);
