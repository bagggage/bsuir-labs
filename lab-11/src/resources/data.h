#pragma once

#include <stdlib.h>

typedef struct Data
{
	unsigned char* buffer;
	size_t size;
} Data;

Data initData();
void destroyData(Data* data);

void pushValueToData(Data* data, void* valuePtr, size_t typeSize);

Data dublicateData(Data* data);
