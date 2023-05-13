#pragma once

#include "../resources/data.h"
#include "../bool.h"

#define BMP_PIXEL_DATA_OFFSET			0x0a
#define BMP_BITS_PER_PIXEL_OFFSET		0x1c
#define BMP_WIDTH_OFFSET				0x12
#define BMP_HEIGHT_OFFSET				0x16
#define BMP_COMPRESSION_MODE_OFFSET		0x1e
#define BMP_COLOR_TABLE_OFFSET			0x36
#define BMP_NUMBER_OF_COLORS_OFFSET		0x2e

#define BMP_COLOR_TABLE_CHANNELS_PER_PIXEL 4

#define BI_RGB 0
#define BI_BITFIELDS 3

typedef enum ChannelMode
{
	MODE_None,
	MODE_RGB,
	MODE_RGBA

} ChannelMode;

#pragma pack(push,1)
typedef struct Pixel 
{
	unsigned char b, g, r, a;

} Pixel;

typedef struct BMPHeader 
{
	unsigned char prefix[2];

	unsigned int size;

	unsigned short applicationSpecifics[2];

	unsigned int pixelDataOffset;

} BMPHeader;

typedef struct DIBHeader 
{
	unsigned int byteSize;

	unsigned int width;
	unsigned int height;

	unsigned short planes;

	unsigned short colorDepth;

	unsigned int compressionMethod;

	unsigned int bitmapDataSize;

	unsigned int horizontalPixelPerMeter;
	unsigned int verticalPixelPerMeter;
	unsigned int colorTableSize;
	unsigned int importantColors;

} DIBHeader;

typedef struct ImageBmp 
{
	BMPHeader header;
	DIBHeader dataHeader;

	Data colorTable;
	Data pixelData;

	BOOL isRaw;

	ChannelMode mode;

} ImageBmp;

ImageBmp initImageBmp();
void destroyImageBmp(ImageBmp* image);

Data getBmpPixelData(BMPHeader* header, DIBHeader* dataHeader, Data* rawData);
ImageBmp makeRawBmpImage(BMPHeader* header, DIBHeader* dataHeader, Data* pixels, unsigned char maxColorDifference);
