#include "post-processing.h"

#include <stdio.h>
#include <math.h>

void applyFilter(ImageBmp* image, Filter filter, void* context) 
{
	FilterContext filterContext =
	{
		.pixels = &image->pixelData,
		.resultPixels = dublicateData(&image->pixelData),
		.mode = image->mode,
		.width = image->dataHeader.width,
		.height = image->dataHeader.height,
		.context = context
	};

	if (filterContext.resultPixels.buffer == NULL)
	{
		printf("\nFilter not applied: dublication data failed\n");
		return;
	}

	for (filterContext.y = 0; filterContext.y < image->dataHeader.height; filterContext.y++)
		for (filterContext.x = 0; filterContext.x < image->dataHeader.width; filterContext.x++)
		{
			filterContext.pixel = (Pixel*)image->pixelData.buffer + (filterContext.y * image->dataHeader.width) + filterContext.x;
			filterContext.resultPixel = (Pixel*)filterContext.resultPixels.buffer + (filterContext.y * image->dataHeader.width) + filterContext.x;

			filter(&filterContext);
		}

	destroyData(&image->pixelData);

	image->pixelData = filterContext.resultPixels;
}

Pixel* getPixel(unsigned int x, unsigned int y, FilterContext* context) 
{
	if (x < 0) x = 0;
	else if (x >= context->width) x = context->width - 1;

	if (y < 0) y = 0;
	else if (y >= context->height) y = context->height - 1;

	return (Pixel*)context->pixels->buffer + (y * context->width) + x;
}

void negate(FilterContext* context) 
{
	context->resultPixel->r = UCHAR_MAX ^ context->pixel->r;
	context->resultPixel->g = UCHAR_MAX ^ context->pixel->g;
	context->resultPixel->b = UCHAR_MAX ^ context->pixel->b;
}

void gammaCorrect(FilterContext* context)
{
	context->resultPixel->r = pow((float)context->pixel->r/ UCHAR_MAX, *(float*)context->context) * UCHAR_MAX;
	context->resultPixel->g = pow((float)context->pixel->g / UCHAR_MAX, *(float*)context->context) * UCHAR_MAX;
	context->resultPixel->b = pow((float)context->pixel->b / UCHAR_MAX, *(float*)context->context) * UCHAR_MAX;
}

void grayscale(FilterContext* context) 
{
	unsigned int value = context->pixel->r + context->pixel->g + context->pixel->b;
	value /= 3;

	context->resultPixel->r = value;
	context->resultPixel->g = value;
	context->resultPixel->b = value;
}

void vinet(FilterContext* context) 
{
	float distanceToCenterX = fabs((context->width / 2.f) - context->x) / ((float)context->width / 2.f);
	float distanceToCenterY = fabs((context->height / 2.f) - context->y) / ((float)context->height / 2.f);
	float distanceToCenter = distanceToCenterX * distanceToCenterX + distanceToCenterY * distanceToCenterY;

	distanceToCenter += 1;

	if (distanceToCenter < 1)
		distanceToCenter = 1;

	context->resultPixel->r /= distanceToCenter;
	context->resultPixel->g /= distanceToCenter;
	context->resultPixel->b /= distanceToCenter;
}

void chromaticAbberation(FilterContext* context) 
{
	context->resultPixel->g = getPixel(context->x + 9, context->y, context)->g;
}

int medianComparator(size_t channelOffset, const unsigned char* lhs, const unsigned char* rhs)
{
	return (lhs[channelOffset] >= rhs[channelOffset] ? 1 : -1);
}

Pixel findMedian(Pixel* pixels, size_t size) 
{
	Pixel medianPixel;

	qsort_s(pixels, size, sizeof(Pixel), &medianComparator, 0);
	medianPixel.r = pixels[size / 2].r;

	qsort_s(pixels, size, sizeof(Pixel), &medianComparator, 1);
	medianPixel.g = pixels[size / 2].g;

	qsort_s(pixels, size, sizeof(Pixel), &medianComparator, 2);
	medianPixel.b = pixels[size / 2].b;

	return medianPixel;
}

void medianFilter(FilterContext* context)
{
	if (context->x == 0 || context->x == context->width - 1 || context->y == 0 || context->y == context->height - 1)
		return;

	Pixel pixels[9];

	for (int i = -1; i <= 1; i++)
	{
		pixels[i + 1] = *getPixel(context->x + i, context->y + 1, context);
		pixels[3 + i + 1] = *getPixel(context->x + i, context->y, context);
		pixels[5 + i + 1] = *getPixel(context->x + i, context->y - 1, context);
	}

	*context->resultPixel = findMedian(pixels, sizeof(pixels) / sizeof(Pixel));
	context->resultPixel->a = context->pixel->a;
}
