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

	const char* left = (context == SortType_NAME ? ((const Videocard*)lhs)->name : ((const Videocard*)lhs)->producer);
	const char* right = (context == SortType_NAME ? ((const Videocard*)rhs)->name : ((const Videocard*)rhs)->producer);

	int i = 0;

	while (left[i] != '\0' && right[i] != '\0')
		if (left[i] > right[i])
			return 1;
		else if (left[i] < right[i])
			return -1;

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
	default:
		assert(0);
		break;
	}
}