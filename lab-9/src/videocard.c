#include "videocard.h"

#include <stdio.h>

void videocardLogInfo(Videocard* card) 
{
	printf("\nVideocard: %s\n", card->name);
	printf("\tVendor: %s\n\tProducer: %s\n\tPrice: %.2f\n\n",
		(card->vendor == VideocardVendor_NVIDIA ? "Nvidia" : "AMD"),
		card->producer,
		card->price
	);
}