#include "data.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

Data initData()
{
	Data data =
	{
		.buffer = NULL,
		.size = 0
	};

	return data;
}

void destroyData(Data* data) 
{
	assert(data != NULL);

	if (data->buffer != NULL)
		free(data->buffer);

	data->buffer = NULL;
	data->size = 0;
}

void pushValueToData(Data* data, void* valuePtr, size_t typeSize)
{
	assert(data != NULL);
	assert(valuePtr != NULL && typeSize > 0);

	if (data->buffer == NULL)
		data->buffer = malloc(typeSize);
	else
		data->buffer = realloc(data->buffer, data->size + typeSize);

	if (data->buffer == NULL)
	{
		perror("Memory allocation failed");
		return;
	}

	if (memcpy_s(data->buffer + data->size, data->size + typeSize, valuePtr, typeSize) != 0)
		perror("\'memcpy\' operation failed");

	data->size += typeSize;
}

Data dublicateData(Data* data) 
{
	Data dublicate = initData();

	dublicate.buffer = malloc(data->size);

	if (dublicate.buffer == NULL) 
	{
		perror("Memory allocation failed");
		return dublicate;
	}

	dublicate.size = data->size;

	if (memcpy_s(dublicate.buffer, dublicate.size, data->buffer, data->size) != 0) 
	{
		perror("\'memcpy\' operation failed");
		free(dublicate.buffer);

		return initData();
	}

	return dublicate;
}
