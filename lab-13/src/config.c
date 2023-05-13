#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define CONFIG_CACHE_SIZE_IN_BYTES_PARAM "cache byte size"
#define CONFIG_CACHE_SIZE_IN_MB_PARAM "cache mb size"
#define CONFIG_MAX_STRING_SIZE 256

Config makeDefaultConfig() 
{
	Config config =
	{
		.maxCacheSize = CONFIG_DEFAULT_MAX_CACHE_SIZE
	};

	return config;
}

Config loadConfig() 
{
	FILE* file;

	errno_t openResult = fopen_s(&file, CONFIG_FILE_NAME, "r");

	if (openResult != 0)
	{
		perror("Can't load config from file");

		Config defaultConfig = makeDefaultConfig();

		saveConfig(&defaultConfig);

		return defaultConfig;
	}

	Config config = makeDefaultConfig();

	char* stringBuffer = (char*)calloc(CONFIG_MAX_STRING_SIZE, sizeof(char));

	if (stringBuffer == NULL)
	{
		perror("Memory allocation failed");
		fclose(file);

		return config;
	}

	while (fgets(stringBuffer, CONFIG_MAX_STRING_SIZE, file) != NULL) 
	{
		char* subString = NULL;
		char* valueString = strstr(stringBuffer, ":");

		if (valueString == NULL)
			continue;

		//Skip ':' symbol
		valueString += 1;

		//Skip spacies
		while (*valueString == ' ' || *valueString == '\t')
			++valueString;

		size_t value = atoll(valueString);

		for (; *valueString != '\0'; ++valueString)
			if (*valueString != ' ' && *valueString != '\t' && *valueString != '\n' && isdigit(*valueString) == false)
			{
				value = 0;
				break;
			}

		if (value == 0)
		{
			printf("Can't read config parameter \"%s\" : unknow symbols \"%s\"\n", stringBuffer, valueString);
			continue;
		}

		if ((subString = strstr(stringBuffer, CONFIG_CACHE_SIZE_IN_BYTES_PARAM)) != NULL)
			config.maxCacheSize = value;
		else if ((subString = strstr(stringBuffer, CONFIG_CACHE_SIZE_IN_MB_PARAM)) != NULL)
			config.maxCacheSize = value * 1024 * 1024;
		else
			printf("Unknown config parameter: \"%s\"\n", stringBuffer);
	}

	free(stringBuffer);

	if (ferror(file) != 0)
	{
		perror("Error while reading config file");
		fclose(file);
	}

	return config;
}

bool saveConfig(const Config* config) 
{
	FILE* file;

	errno_t openResult = fopen_s(&file, CONFIG_FILE_NAME, "w");

	if (openResult != 0) 
	{
		perror("Can't save config in file");
		return false;
	}

	int writenBytes = fprintf_s(file, "%s: %llu", CONFIG_CACHE_SIZE_IN_BYTES_PARAM, config->maxCacheSize);

	if (writenBytes < 0)
	{
		perror("Can't write config in file");
		fclose(file);

		return false;
	}

	fclose(file);

	return true;
}
