#include "queue.h"

#include <assert.h>

PriorityQueue initQueue()
{
	PriorityQueue queue =
	{
		.size = 0,
		.head = NULL,
		.tail = NULL
	};

	return queue;
}

void destructQueue(PriorityQueue* queue)
{
	assert(queue != NULL);

	QueueElement* currentElement = queue->head;

	while (currentElement != NULL)
	{
		QueueElement* temp = currentElement;

		currentElement = currentElement->next;

		free(temp->value);
		free(temp);
	}

	queue->size = 0;
	queue->head = NULL;
	queue->tail = NULL;
}

bool enQueue(PriorityQueue* queue, Domain* value)
{
	assert(queue != NULL);
	assert(value != NULL);

	QueueElement* element = (QueueElement*)calloc(1, sizeof(QueueElement));

	if (element == NULL) 
	{
		perror("Can't add element in queue, memory allocation failed");
		return false;
	}

	element->value = value;

	QueueElement* temp = queue->head;

	queue->head = element;
	element->next = temp;

	if (temp != NULL)
		temp->prev = element;
	else
		queue->tail = element;

	++queue->size;

	return true;
}

void prioritizeQueueElement(PriorityQueue* queue, QueueElement* element) 
{
	assert(queue != NULL);
	assert(element != NULL);

	if (queue->head == element)
		return;

	QueueElement* temp = queue->head;

	queue->head = element;
	element->prev->next = element->next;
	
	if (element->next != NULL)
		element->next->prev = element->prev;

	if (queue->tail == element)
		queue->tail = element->prev;

	element->next = temp;
	element->prev = NULL;

	temp->prev = element;
}

void deQueueHead(PriorityQueue* queue) 
{
	assert(queue != NULL);

	if (queue->head == NULL)
		return;

	QueueElement* temp = queue->head;

	if (queue->head == queue->tail) 
	{
		queue->head = NULL;
		queue->tail = NULL;
	}
	else
	{
		queue->head = temp->next;
		queue->head->prev = NULL;
	}

	destructDomain(temp->value);
	free(temp->value);
	free(temp);

	--queue->size;
}

void deQueue(PriorityQueue* queue) 
{
	assert(queue != NULL);

	if (queue->tail == NULL)
		return;

	QueueElement* temp = queue->tail;

	if (queue->head == queue->tail)
	{
		queue->head = NULL;
		queue->tail = NULL;
	}
	else
	{
		queue->tail = temp->prev;
		queue->tail->next = NULL;
	}

	destructDomain(temp->value);
	free(temp->value);
	free(temp);

	--queue->size;
}
