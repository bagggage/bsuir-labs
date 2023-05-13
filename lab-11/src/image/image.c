#include "image.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#define ONE_BIT_MASK	0b10000000
#define TWO_BIT_MASK	0b11000000
#define FOUR_BIT_MASK	0b11110000

#define RED16_MASK		0x7C00
#define GREEN16_MASK	0x3E0
#define BLUE16_MASK		0x1F
#define ALPHA16_MASK	0x00

#define TRUNCATE(x)  ((x) >> 3)
#define ARGB16(a,r,g,b) ((a << 15) | (TRUNCATE(r) << 10) | (TRUNCATE(g) << 5) | TRUNCATE(b))

#define ARGB16_MASKS_COUNT 4

#define DWORD_SIZE 4

ImageBmp initImageBmp()
{
	ImageBmp image =
	{
		.colorTable = initData(),
		.pixelData = initData(),
		.mode = MODE_None,
	};

	return image;
}

void destroyImageBmp(ImageBmp* image) 
{
	assert(image != NULL);

	destroyData(&image->colorTable);
	destroyData(&image->pixelData);

	*image = initImageBmp();
}

inline unsigned int roundToBigger(float value) 
{
	return ((unsigned int)value < value ? (unsigned int)value + 1 : (unsigned int)value);
}

Data getBmpPixelData(BMPHeader* header, DIBHeader* dataHeader, Data* rawData) 
{
	assert(header != NULL);
	assert(dataHeader != NULL);
	assert(rawData != NULL && rawData->buffer != NULL);

	Data data = initData();

	size_t pixelsCount = dataHeader->width * dataHeader->height;

	data.size = sizeof(Pixel) * pixelsCount;
	data.buffer = malloc(data.size);

	if (data.buffer == NULL)
	{
		perror("Memory allocation error");
		return initData();
	}

	unsigned char* pixels = rawData->buffer + header->pixelDataOffset;
	unsigned char* colorTable = rawData->buffer + BMP_COLOR_TABLE_OFFSET;
	unsigned int byteOffsetPerRow = roundToBigger(dataHeader->width * ((float)dataHeader->colorDepth / CHAR_BIT));

	byteOffsetPerRow = DWORD_SIZE - (byteOffsetPerRow % DWORD_SIZE);
	byteOffsetPerRow = (byteOffsetPerRow == DWORD_SIZE ? 0 : byteOffsetPerRow);

	if (dataHeader->colorDepth <= CHAR_BIT)
	{
		for (int i = 0; i < pixelsCount; i++)
		{
			unsigned char bit = 0;
			unsigned int pixelIndex = 0;
			unsigned int rowIndex = (byteOffsetPerRow > 0 ? (i / dataHeader->width) : 0);
			unsigned int offset = rowIndex * byteOffsetPerRow;

			switch (dataHeader->colorDepth)
			{
			case 1:
				bit = (i % CHAR_BIT + rowIndex);
				pixelIndex = pixels[i / CHAR_BIT + offset] & (ONE_BIT_MASK >> bit) >> (CHAR_BIT - bit - 1);
				break;
			case 2:
				bit = (i % (CHAR_BIT / 2)) * 2;
				pixelIndex = (pixels[i / (CHAR_BIT / 2) + offset] & (TWO_BIT_MASK >> bit)) >> (CHAR_BIT - bit - 2);
				break;
			case 4:
				bit = ((i + rowIndex) % (CHAR_BIT / 4)) * 4;
				pixelIndex = (pixels[(i + rowIndex) / (CHAR_BIT / 4) + offset] & (FOUR_BIT_MASK >> bit)) >> (CHAR_BIT - bit - 4);
				break;
			case 8:
				pixelIndex = pixels[i + offset];
				break;
			default:
				assert(0);
				break;
			}

			Pixel currentPixel;

			pixelIndex *= 4;

			currentPixel.r = colorTable[pixelIndex + 2];
			currentPixel.g = colorTable[pixelIndex + 1];
			currentPixel.b = colorTable[pixelIndex + 0];
			currentPixel.a = colorTable[pixelIndex + 3];

			((Pixel*)data.buffer)[i] = currentPixel;
		}
	}
	else 
	{
		for (int i = 0; i < pixelsCount; i++)
		{
			unsigned int channelIndex = i;
			unsigned int offset = (i / dataHeader->width) * byteOffsetPerRow;

			Pixel currentPixel;

			switch (dataHeader->colorDepth)
			{
			case 32:
				channelIndex *= 4;

				currentPixel.r = pixels[channelIndex + 2];
				currentPixel.g = pixels[channelIndex + 1];
				currentPixel.b = pixels[channelIndex];
				currentPixel.a = pixels[channelIndex + 3];
				break;
			case 24:
				channelIndex *= 3;
				channelIndex += offset;

				currentPixel.r = pixels[channelIndex + 2];
				currentPixel.g = pixels[channelIndex + 1];
				currentPixel.b = pixels[channelIndex];
				currentPixel.a = UCHAR_MAX;
				break;
			case 16:
				channelIndex += offset;

				currentPixel.r = ((((unsigned short*)pixels)[channelIndex] & ((unsigned int*)colorTable)[0]) >> 11) << 3;
				currentPixel.g = ((((unsigned short*)pixels)[channelIndex] & ((unsigned int*)colorTable)[1]) >> 5) << 2;
				currentPixel.b = ((((unsigned short*)pixels)[channelIndex] & ((unsigned int*)colorTable)[2])) << 3;
				currentPixel.a = ((((unsigned short*)pixels)[channelIndex] & ((unsigned int*)colorTable)[3]) >> 15);
				break;
			default:
				assert(0);
				break;
			}

			((Pixel*)data.buffer)[i] = currentPixel;
		}
	}

	return data;
}

unsigned char findMostSimilarColorInTable(Pixel color, Data* colors) 
{
	unsigned char similar = 0;

	unsigned short tempDifference = USHRT_MAX;

	for (unsigned int i = 0; i < (colors->size / sizeof(Pixel)); i++)
	{
		unsigned short currentDifference = 0;

		currentDifference += abs(((Pixel*)colors->buffer)[i].r - color.r);
		currentDifference += abs(((Pixel*)colors->buffer)[i].g - color.g);
		currentDifference += abs(((Pixel*)colors->buffer)[i].b - color.b);
		currentDifference += abs(((Pixel*)colors->buffer)[i].a - color.a);

		if (currentDifference < tempDifference)
		{
			tempDifference = currentDifference;
			similar = i;
		}
	}

	return similar;
}

typedef struct PixelCount 
{
	Pixel pixel;
	unsigned int count;

} PixelCount;

BOOL isPixelsVerySimilar(Pixel lhs, Pixel rhs, unsigned char maxColorDifference)
{
	unsigned char differenceR = abs(lhs.r - rhs.r);
	unsigned char differenceG = abs(lhs.g - rhs.g);
	unsigned char differenceB = abs(lhs.b - rhs.b);

	return (differenceR < maxColorDifference) && (differenceG < maxColorDifference) && (differenceB < maxColorDifference);
}

void countPixelWithPixelCounter(Data* pixelCounter, Pixel pixel, unsigned char maxColorDifference) 
{
	for (unsigned int i = 0; i < (pixelCounter->size / sizeof(PixelCount)); i++)
		if (isPixelsVerySimilar(((PixelCount*)pixelCounter->buffer)[i].pixel, pixel, maxColorDifference))
		{
			((PixelCount*)pixelCounter->buffer)[i].count++;
			return;
		}

	PixelCount count = { .pixel = pixel, .count = 1 };

	pushValueToData(pixelCounter, &count, sizeof(PixelCount));
}

int pixelCountComaparator(void* context, PixelCount* lhs, PixelCount* rhs) 
{
	return (lhs->count >= rhs->count ? 1 : -1);
}

Data makeColorTable(DIBHeader* dataHeader, Data* pixels, unsigned int maxColorDifference) 
{
	Data colorTable = initData();

	colorTable.size = pow(2, dataHeader->colorDepth) * sizeof(char[4]);
	colorTable.buffer = calloc(colorTable.size, sizeof(char));

	if (colorTable.buffer == NULL)
	{
		perror("Memory allocation failed");
		return initData();
	}

	Data pixelCounter = initData();

	for (unsigned int i = 0; i < (pixels->size / sizeof(Pixel)); i++)
		countPixelWithPixelCounter(&pixelCounter, ((Pixel*)pixels->buffer)[i], maxColorDifference);

	unsigned int pixelCounterElementsCount = pixelCounter.size / sizeof(PixelCount);
	unsigned int colorTableColorsCount = colorTable.size / sizeof(char[4]);

	int resultColorTableSize = (colorTableColorsCount > pixelCounterElementsCount ? pixelCounterElementsCount : colorTableColorsCount);

	if (resultColorTableSize < pixelCounterElementsCount)
		qsort_s(pixelCounter.buffer, pixelCounterElementsCount, sizeof(PixelCount), &pixelCountComaparator, NULL);

	for (unsigned int i = 0; i < resultColorTableSize; i++)
	{
		unsigned int tempIndex = i * 4;
		
		colorTable.buffer[tempIndex] = ((PixelCount*)pixelCounter.buffer)[i].pixel.b;
		colorTable.buffer[tempIndex + 1] = ((PixelCount*)pixelCounter.buffer)[i].pixel.g;
		colorTable.buffer[tempIndex + 2] = ((PixelCount*)pixelCounter.buffer)[i].pixel.r;
		colorTable.buffer[tempIndex + 3] = 0;
	}

	destroyData(&pixelCounter);

	return colorTable;
}

ImageBmp makeRawBmpImage(BMPHeader* header, DIBHeader* dataHeader, Data* pixels, unsigned char maxColorDifference) 
{
	assert(header != NULL);
	assert(dataHeader != NULL);
	assert(pixels != NULL);

	ImageBmp image = initImageBmp();

	image.header = *header;
	image.dataHeader = *dataHeader;
	image.dataHeader.colorTableSize = 0;
	image.isRaw = TRUE;

	unsigned int byteOffsetPerRow = roundToBigger(dataHeader->width * ((float)dataHeader->colorDepth / CHAR_BIT));

	image.pixelData.size = dataHeader->height * byteOffsetPerRow;

	byteOffsetPerRow = DWORD_SIZE - (byteOffsetPerRow % DWORD_SIZE);
	byteOffsetPerRow = (byteOffsetPerRow == DWORD_SIZE ? 0 : byteOffsetPerRow);

	image.pixelData.size += dataHeader->height * byteOffsetPerRow;
	image.pixelData.buffer = calloc(image.pixelData.size, sizeof(char));

	if (image.pixelData.buffer == NULL) 
	{
		perror("Memory allocation failed");
		return initImageBmp();
	}

	if (dataHeader->colorDepth <= CHAR_BIT) 
	{
		image.colorTable = makeColorTable(dataHeader, pixels, maxColorDifference);

		if (image.colorTable.buffer == NULL)
		{
			destroyData(&image.pixelData);
			return initImageBmp();
		}

		for (int i = 0; i < pixels->size / sizeof(Pixel); i++)
		{
			unsigned int tempIndex = i;
			unsigned int rowIndex = (byteOffsetPerRow > 0 ? (i / dataHeader->width) : 0);
			unsigned int offset = (i / dataHeader->width) * byteOffsetPerRow;
			unsigned int bit;

			switch (dataHeader->colorDepth)
			{
			case 1:
				tempIndex = (tempIndex + rowIndex) / CHAR_BIT;
				tempIndex += offset;
				bit = ((i + rowIndex) % CHAR_BIT);
				image.pixelData.buffer[tempIndex] |= 
					findMostSimilarColorInTable(((Pixel*)pixels->buffer)[i], &image.colorTable) << (CHAR_BIT - bit - 1);
				break;
			case 2:
				tempIndex = (tempIndex + rowIndex) / (CHAR_BIT / 2);
				tempIndex += offset;
				bit = ((i + rowIndex) % (CHAR_BIT / 2)) * 2;
				image.pixelData.buffer[tempIndex] |= 
					findMostSimilarColorInTable(((Pixel*)pixels->buffer)[i], &image.colorTable) << (CHAR_BIT - bit - 2);
				break;
			case 4:
				tempIndex = (tempIndex + rowIndex) / (CHAR_BIT / 4);
				tempIndex += offset;
				bit = ((i + rowIndex) % (CHAR_BIT / 4)) * 4;
				image.pixelData.buffer[tempIndex] |=
					findMostSimilarColorInTable(((Pixel*)pixels->buffer)[i], &image.colorTable) << (CHAR_BIT - bit - 4);
				break;
			case 8:
				image.pixelData.buffer[i + offset] = findMostSimilarColorInTable(((Pixel*)pixels->buffer)[i], &image.colorTable);
				break;
			default:
				assert(0);
				break;
			}
		}
	}
	else 
	{
		for (int i = 0; i < pixels->size / sizeof(Pixel); i++)
		{
			unsigned int tempIndex = i;
			unsigned int offset = (i / dataHeader->width) * byteOffsetPerRow;

			switch (dataHeader->colorDepth)
			{
			case 32:
				((Pixel*)image.pixelData.buffer)[tempIndex] = ((Pixel*)pixels->buffer)[i];
				break;
			case 24:
				tempIndex *= 3;
				tempIndex += offset;

				image.pixelData.buffer[tempIndex + 2] = ((Pixel*)pixels->buffer)[i].r;
				image.pixelData.buffer[tempIndex + 1] = ((Pixel*)pixels->buffer)[i].g;
				image.pixelData.buffer[tempIndex + 0] = ((Pixel*)pixels->buffer)[i].b;
				break;
			case 16:
			{
				Pixel currentPixel = ((Pixel*)pixels->buffer)[i];
					
				((unsigned short*)(image.pixelData.buffer + offset))[tempIndex] = ARGB16(currentPixel.a, currentPixel.r, currentPixel.g, currentPixel.b);
			}
				break;
			default:
				assert(0);
				break;
			}
		}
	}

	image.dataHeader.byteSize = sizeof(DIBHeader);
	image.dataHeader.compressionMethod = (image.dataHeader.colorDepth == 16 ? BI_BITFIELDS : BI_RGB);

	if (image.dataHeader.compressionMethod == BI_RGB)
	{
		image.dataHeader.colorTableSize = image.colorTable.size / sizeof(char[4]);
	}
	else
	{
		image.dataHeader.colorTableSize = 0;
		image.colorTable.size = ARGB16_MASKS_COUNT * sizeof(unsigned int);
		image.colorTable.buffer = malloc(image.colorTable.size);

		if (image.colorTable.buffer == NULL) 
		{
			perror("Memory allocation failed");

			destroyData(&image.pixelData);

			return initImageBmp();
		}

		((unsigned int*)image.colorTable.buffer)[0] = RED16_MASK;
		((unsigned int*)image.colorTable.buffer)[1] = GREEN16_MASK;
		((unsigned int*)image.colorTable.buffer)[2] = BLUE16_MASK;
		((unsigned int*)image.colorTable.buffer)[3] = ALPHA16_MASK;
	}

	image.header.pixelDataOffset = sizeof(BMPHeader) + sizeof(DIBHeader) + image.colorTable.size;
	image.header.size = image.header.pixelDataOffset + image.pixelData.size;

	image.dataHeader.bitmapDataSize = (byteOffsetPerRow > 0 ? image.pixelData.size : 0);

	return image;
}
