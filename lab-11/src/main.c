#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#include "resources/resources.h"
#include "image/post-processing.h"
#include "console-menu/menu.h"

throwErrorAndClearInputBuffer(const char* msg) 
{
	printf("%s\n", msg);
	rewind(stdin);
}

char* readString() 
{
	rewind(stdin);

	char* string = calloc(MAX_PATH, sizeof(char));

	if (string == NULL) 
	{
		perror("Memory allocation failed");
		return NULL;
	}

	if (gets_s(string, MAX_PATH) == 0)
	{
		free(string);
		return NULL;
	}

	size_t length = strnlen_s(string, MAX_PATH - 1);
	
	string = realloc(string, length + 1);

	if (string == NULL)
	{
		perror("Memory allocation failed");
		return NULL;
	}

	string[length] = '\0';

	return string;
}

void applyGammaCorrectionWithInputStreamData(ImageBmp* image) 
{
	printf("Enter gamma value: ");

	float gamma;

	while (scanf_s("%f", &gamma) < 1 || gamma <= 0 || gamma > 20) 
		throwErrorAndClearInputBuffer("\nIncorrect input format, the number sould be a floating point (0 < x <= 20)");

	rewind(stdin);

	float inverseGamma = 1.f / gamma;

	applyFilter(image, &gammaCorrect, &inverseGamma);
}

int main()
{
	printf("Enter image path: ");
	char* sourceImagePath = readString();

	if (sourceImagePath == NULL)
		return -1;

	system("cls");

	ImageBmp image = loadBmpImage(sourceImagePath);

	if (image.pixelData.buffer == NULL)
		return -1;

	Menu menu = initMenu();
	Menu subMenu = initMenu();

	subMenu.isSubmenu = TRUE;

	float inversGamma = 1.f / 2.2f;

	addMenuItem(&subMenu, "Grayscale", funcBind(&applyFilter, 3, &image, &grayscale, NULL));
	addMenuItem(&subMenu, "Negative", funcBind(&applyFilter, 3, &image, &negate, NULL));
	addMenuItem(&subMenu, "Gamma Correction", funcBind(&applyGammaCorrectionWithInputStreamData, 1, &image));
	addMenuItem(&subMenu, "Chromatic Abberation", funcBind(&applyFilter, 3, &image, &chromaticAbberation, NULL));
	addMenuItem(&subMenu, "Median Filter", funcBind(&applyFilter, 3, &image, &medianFilter, NULL));
	addMenuItem(&subMenu, "Vinet", funcBind(&applyFilter, 3, &image, &vinet, NULL));

	addSubmenu(&menu, "Apply filter", &subMenu);

	while (handleMenuInteraction(&menu) == 0)
	{
		printf("\nDo you want to conitnue? (Y\\N)\n");

		char choise;

		while (scanf_s("%c", &choise, 1) < 1 || (choise != 'Y' && choise != 'N'))
			throwErrorAndClearInputBuffer("\nIncorrect input, please choose (Y\\N): ");

		if (choise == 'N')
			break;

		system("cls");
	}

	unsigned int maxColorDifference = 1;

	if (image.dataHeader.colorDepth <= CHAR_BIT) 
	{
		printf("This image has low-bit color depth\nTo choose color for color table, please enter max color difference: ");

		while(scanf_s("%d", &maxColorDifference) < 0) 
			throwErrorAndClearInputBuffer("Incorrect input format, the value shoulbe integer (0 < x < 256), try again: ");
	}

	ImageBmp rawImage = makeRawBmpImage(&image.header, &image.dataHeader, &image.pixelData, maxColorDifference);

	if (rawImage.pixelData.buffer != NULL)
		saveBmpImage(&rawImage, "changed.bmp");
	else
		printf("Image saving failed: image is empty");

	destroyMenu(&menu);
	destroyMenu(&subMenu);

	destroyData(&image.pixelData);
	destroyData(&rawImage.pixelData);

	if (rawImage.colorTable.size > 0)
		destroyData(&rawImage.colorTable);

	return 0;
}
