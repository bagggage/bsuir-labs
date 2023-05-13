#pragma once

#include "domain.h"

#include <stdlib.h>
#include <stdbool.h>

typedef struct QueueElement
{
	Domain* value;

	struct QueueElement* next;
	struct QueueElement* prev;

} QueueElement;

typedef struct PriorityQueue 
{
	size_t size;

	QueueElement* head;
	QueueElement* tail;
	
} PriorityQueue;

PriorityQueue initQueue();

void destructQueue(PriorityQueue* queue);

bool enQueue(PriorityQueue* queue, Domain* value);
void deQueue(PriorityQueue* queue);
void deQueueHead(PriorityQueue* queue);
void prioritizeQueueElement(PriorityQueue* queue, QueueElement* element);
