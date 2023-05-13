#pragma once

#include "data.h"
#include "../image/image.h"

Data loadDataFromFile(const char* filePath);

void saveDataToFile(const Data* data, const char* filePath);

ImageBmp loadBmpImage(const char* imagePath);

void saveBmpImage(ImageBmp* image, const char* imagePath);
