#pragma once

#ifndef DOMAIN_NAME_AVARAGE_LENGTH
#define DOMAIN_NAME_AVARAGE_LENGTH 12
#endif // !DOMAIN_NAME_AVARAGE_LENGTH

typedef struct Domain 
{
	char* name;
	unsigned char ip[4];

} Domain;

Domain initDomain();
Domain makeDomain(const char* name, const unsigned char ip[4]);

void destructDomain(Domain* domain);

void printDomain(const Domain* domain);
void printIp(const unsigned char ip[4]);
