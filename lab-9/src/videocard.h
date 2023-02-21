#pragma once

typedef enum VideocardVendor 
{
	VideocardVendor_NVIDIA,
	VideocardVendor_AMD

} VideocardVendor;

typedef enum SortType
{
	SortType_PRICE,
	SortType_NAME,
	SortType_PRODUCER,
	SortType_VENDOR

} SortType;


typedef struct Videocard
{
	VideocardVendor vendor;
	char* name;
	char* producer;
	float price;

} Videocard;

void videocardLogInfo(const Videocard* card);
void sortVideocards(Videocard* cards, size_t count, SortType sortType);