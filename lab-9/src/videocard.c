#include "videocard.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

void videocardLogInfo(const Videocard* card) 
{
	printf("%s\n%s %s\nPrice: %.2f BYN\n\n",
		card->name,
		(card->vendor == VideocardVendor_NVIDIA ? "Nvidia" : "AMD"),
		card->producer,
		card->price
	);
}

void removeVideocardFromArray(Videocard** array, size_t* size, unsigned int index)
{
	assert(array != NULL);
	assert(*size != NULL);

	if (index >= *size)
		return;

	for (int i = index; i < *size - 1; i++)
		(*array)[i] = (*array)[i + 1];

	(*size)--;

	(*array) = realloc(*array, (*size) * sizeof(Videocard));

	if (*array == NULL)
		exit(2);
}

int priceComparator(void* context, const void* lhs, const void* rhs) 
{
	if (((const Videocard*)lhs)->price > ((const Videocard*)rhs)->price)
		return 1;
	else
		return -1;
}

int vendorComparator(void* context, const void* lhs, const void* rhs) 
{
	if (((const Videocard*)lhs)->vendor > ((const Videocard*)rhs)->vendor)
		return 1;
	else
		return -1;
}

int strComparator(void* context, const void* lhs, const void* rhs) 
{
	SortType type = *(SortType*)context;

	const char* left = (type == SortType_NAME ? ((const Videocard*)lhs)->name : ((const Videocard*)lhs)->producer);
	const char* right = (type == SortType_NAME ? ((const Videocard*)rhs)->name : ((const Videocard*)rhs)->producer);

	int i = 0;

	while (left[i] != '\0' && right[i] != '\0')
	{
		if (left[i] > right[i])
			return 1;
		else if (left[i] < right[i])
			return -1;

		i++;
	}

	return -1;
}

int vendorPriceComparator(void* context, const void* lhs, const void* rhs) 
{
	SortType type = *(SortType*)context;

	const Videocard* left = (const Videocard*)lhs;
	const Videocard* right = (const Videocard*)rhs;

	if (left->vendor > right->vendor)
		return 1;
	else if (left->vendor < right->vendor)
		return -1;
	else if (left->price > right->price)
		return 1;
	else
		return -1;
}

void sortVideocards(Videocard* cards, size_t count, SortType sortType)
{
	switch (sortType)
	{
	case SortType_NAME:
		qsort_s(cards, count, sizeof(Videocard), &strComparator, &sortType);
		break;
	case SortType_PRODUCER:
		qsort_s(cards, count, sizeof(Videocard), &strComparator, &sortType);
		break;
	case SortType_PRICE:
		qsort_s(cards, count, sizeof(Videocard), &priceComparator, &sortType);
		break;
	case SortType_VENDOR:
		qsort_s(cards, count, sizeof(Videocard), &vendorComparator, &sortType);
		break;
	case SortType_PRICE_VENDOR:
		qsort_s(cards, count, sizeof(Videocard), &vendorPriceComparator, &sortType);
		break;
	default:
		assert(0);
		break;
	}
}