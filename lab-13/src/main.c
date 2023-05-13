#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "config.h"
#include "dns/cache.h"

#define DNS_DATABASE_PATH "dns.txt"
#define STRING_BUFFER_SIZE 256

void configTest(DNSCache* cache) 
{
	QueueElement* temp = cache->queue.head;

	while (temp != NULL)
	{
		printf("%s", temp->value->name);

		if (temp->next)
			printf(" -> ");

		temp = temp->next;
	}

	printf("\n");
}

void testDns(DNSCache* cache) 
{
	char* stringBuffer = (char*)calloc(STRING_BUFFER_SIZE, sizeof(char));

	if (stringBuffer == NULL)
	{
		perror("Testing breaked, memory allocation failed");
		return;
	}

	while (true) 
	{
		if (gets_s(stringBuffer, STRING_BUFFER_SIZE) == NULL)
		{
			printf("Testing breaked, reading domain name  failed\n");
			free(stringBuffer);
			return;
		}

		if (*stringBuffer == '\0')
			continue;

		if (strcmp(stringBuffer, "exit") == 0)
			break;

		if (*stringBuffer == '~')
		{
			char* nextToken = NULL;
			char* name = strtok_s(stringBuffer + 1, " ", &nextToken);
			char* ip = strtok_s(NULL, " ", &nextToken);

			if (isStringIpCorrect(ip) == false)
			{
				printf("Unknow domain format\n");
				continue;
			}

			if (addDnsDomain(cache, name, ip))
				printf("Domain \"%s: %s\" added\n", name, ip);
			else
				printf("Domain wasn't added successfull, something went wrong\n");

			continue;
		}

		if (*stringBuffer == '*')
		{
			configTest(cache);
			continue;
		}

		if (isStringIpCorrect(stringBuffer)) 
		{
			unsigned int size;
			Domain** domains = getDnsDomainsByIp(cache, stringBuffer, &size);

			if (domains != NULL)
			{
				for (int i = 0; i < size; ++i)
					printDomain(domains[i]);

				free(domains);
			}
			else 
			{
				printf("Domains with Ip \"%s\" not found\n", stringBuffer);
			}

			continue;
		}

		const Domain* domain = getDnsDomainByName(cache, stringBuffer);

		if (domain != NULL)
			printIp(domain->ip);
		else
			printf("Ip for domain \"%s\" not found\n", stringBuffer);
	}

	free(stringBuffer);
}

int main() 
{
	Config config = loadConfig();
	DNSCache cache = initDnsCache(config.maxCacheSize / getDnsCacheElementSize(), DNS_DATABASE_PATH);

	testDns(&cache);

	destructDnsCache(&cache);

	return 0;
}
