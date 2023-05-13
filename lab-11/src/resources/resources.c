#include "resources.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "../error.h"

Data loadDataFromFile(const char* filePath) 
{
	FILE* file;
	Data data = initData();

	if (fopen_s(&file, filePath, "rb") != 0)
	{
		perror("File open error");
		return data;
	}
	
	fseek(file, 0, SEEK_END);

	data.size = ftell(file);
	data.buffer = malloc(data.size);

	if (data.buffer == NULL)
	{
		perror("Memory allocation failed");
		fclose(file);

		return data;
	}

	fseek(file, 0, SEEK_SET);

	data.size = fread_s(data.buffer, data.size, sizeof(char), data.size, file);

	if (ferror(file) != 0)
	{
		perror("File reading error");
		fclose(file);
		free(data.buffer);

		data.size = 0;

		return data;
	}

	fclose(file);

	void* reallocatedBuffer = realloc(data.buffer, data.size);

	if (reallocatedBuffer == NULL) 
	{
		perror("Memory allocation failed");
		free(data.buffer);

		data.size = 0;

		return data;
	}

	data.buffer = reallocatedBuffer;

	return data;
}

void saveDataToFile(const Data* data, const char* filePath) 
{
	assert(data != NULL);

	FILE* file;

	if (fopen_s(&file, filePath, "wb") != 0)
	{
		perror("File open error");
		return;
	}

	fwrite(data->buffer, sizeof(char), data->size, file);

	if (ferror(file) != 0)
	{
		perror("File writing error");
		fclose(file);

		return;
	}

	fclose(file);
}

ImageBmp loadBmpImage(const char* imagePath) 
{
	Data data = loadDataFromFile(imagePath);

	if (data.buffer == NULL)
	{
		throwError("File data is empty");
		return initImageBmp();
	}
	else if (((char*)data.buffer)[0] != 'B' || ((char*)data.buffer)[1] != 'M')
	{
		throwError("Incorrect file format");
		destroyData(&data);
		return initImageBmp();
	}

	ImageBmp image;

	image.header = *(BMPHeader*)(data.buffer);
	image.dataHeader = *(DIBHeader*)(data.buffer + sizeof(BMPHeader));

	if (image.dataHeader.compressionMethod != BI_RGB && image.dataHeader.compressionMethod != BI_BITFIELDS)
	{
		throwError("Incorrect file format");
		destroyData(&data);
		return initImageBmp();
	}

	if (image.dataHeader.colorDepth <= CHAR_BIT) 
		image.mode = MODE_RGB;
	else 
		image.mode = ((image.dataHeader.colorDepth / CHAR_BIT) > 3 ? MODE_RGBA : MODE_RGB);

	image.pixelData = getBmpPixelData(&image.header, &image.dataHeader, &data);
	image.colorTable = initData();
	image.isRaw = FALSE;

	destroyData(&data);

	return image;
}

void saveBmpImage(ImageBmp* image, const char* imagePath)
{
	assert(image != NULL);
	assert(image->isRaw);

	FILE* file;

	if (fopen_s(&file, imagePath, "wb") != 0)
	{
		perror("File open error");
		return;
	}

	fwrite(&image->header, sizeof(BMPHeader), 1, file);
	fwrite(&image->dataHeader, sizeof(DIBHeader), 1, file);

	if (image->colorTable.buffer != NULL)
		fwrite(image->colorTable.buffer, sizeof(char), image->colorTable.size, file);

	fwrite(image->pixelData.buffer, sizeof(char), image->pixelData.size, file);

	fclose(file);
}
