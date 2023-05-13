#pragma once

#include <stdbool.h>

#ifndef CONFIG_FILE_NAME
#define CONFIG_FILE_NAME "config.ini"
#endif // !CONFIG_FILE_NAME

#ifndef CONFIG_DEFAULT_MAX_CACHE_SIZE
#define CONFIG_DEFAULT_MAX_CACHE_SIZE 1024
#endif // !CONFIG_DEFAULT_MAX_CACHE_SIZE

typedef struct Config 
{
	size_t maxCacheSize;
} Config;

Config loadConfig();

bool saveConfig(const Config* config);
