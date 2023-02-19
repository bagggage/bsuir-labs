#pragma once

typedef enum VideocardVendor 
{
	VideocardVendor_NVIDIA,
	VideocardVendor_AMD

} VideocardVendor;

typedef struct Videocard
{
	VideocardVendor vendor;
	char* name;
	char* producer;
	float price;

} Videocard;

void videocardLogInfo(Videocard* card);