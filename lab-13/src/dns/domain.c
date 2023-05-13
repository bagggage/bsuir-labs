#include "domain.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

Domain initDomain() 
{
	Domain domain;

	domain.name = NULL;
	*(unsigned int*)domain.ip = 0;

	return domain;
}

Domain makeDomain(const char* name, unsigned char ip[4])
{
	assert(name != NULL);

	Domain domain;

	unsigned int nameLength = strnlen_s(name, SHRT_MAX);

	domain.name = malloc(nameLength + 1);

	if (domain.name == NULL) 
	{
		perror("Can't create domain, memory allocation failed");

		*(unsigned int*)domain.ip = 0;

		return domain;
	}

	strcpy_s(domain.name, nameLength + 1, name);

	*(unsigned int*)domain.ip = *(unsigned int*)ip;

	return domain;
}

void destructDomain(Domain* domain) 
{
	assert(domain != NULL);

	if (domain->name != NULL)
		free(domain->name);

	domain->name = NULL;
	*(unsigned int*)domain->ip = 0;
}

void printDomain(const Domain* domain) 
{
	printf("%s: ", domain->name);
	printIp(domain->ip);
}

void printIp(const unsigned char ip[4]) 
{
	printf("%u.%u.%u.%u\n", (unsigned int)ip[0], (unsigned int)ip[1], (unsigned int)ip[2], (unsigned int)ip[3]);
}
