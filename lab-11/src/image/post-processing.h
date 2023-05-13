#pragma once

#include "image.h"

typedef struct FilterContext
{
	Data* pixels;
	Data resultPixels;
	ChannelMode mode;

	Pixel* pixel;
	Pixel* resultPixel;

	unsigned int x, y;
	unsigned int width, height;

	void* context;
} FilterContext;

typedef void (*Filter)(FilterContext*);

void applyFilter(ImageBmp* image, Filter filter, void* context);

Pixel* getPixel(unsigned int x, unsigned int y, FilterContext* context);

void negate(FilterContext* context);
void gammaCorrect(FilterContext* context);
void grayscale(FilterContext* context);
void vinet(FilterContext* context);
void chromaticAbberation(FilterContext* context);
void medianFilter(FilterContext* context);
