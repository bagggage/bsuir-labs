#include "stack.h"

#include <assert.h>

#include "../exception.h"

void copy(void* dst, const void* src, size_t size) 
{
	for (int i = 0; i < size; i++)
		((char*)dst)[i] = ((const char*)src)[i];
}

Stack stackInit() 
{
	Stack stack =
	{
		.first = NULL,
		.top = NULL
	};

	return stack;
}

void stackPush(Stack* stack, const void* stackElement, size_t elementSize) 
{
	assert(stack != NULL);
	assert(stackElement != NULL && elementSize >= sizeof(StackElement));

	StackElement* elementPtr = (StackElement*)malloc(elementSize);

	if (elementPtr == NULL)
		 throwException(1, "Memory allocation failed, stack overflow");

	copy(elementPtr, stackElement, elementSize);

	if (stack->first == NULL)
	{
		stack->top = elementPtr;
		stack->first = elementPtr;
		stack->first->previous = NULL;
	}
	else
	{
		elementPtr->previous = stack->top;
		stack->top = elementPtr;
	}
}

void stackPop(Stack* stack) 
{
	assert(stack != NULL);

	if (stack->top == NULL)
		throwException(2, "Stack underflow");

	StackElement* top = stack->top;

	stack->top = top->previous;

	if (stack->top == NULL)
		stack->first = NULL;

	free(top);
}

void stackRemove(Stack* stack, StackElement* element) 
{
	StackElement* upper = NULL;
	StackElement* current = stack->top;

	while (current != NULL)
	{
		if (current == element)
		{
			if (upper != NULL)
				upper->previous = current->previous;
			else
				stack->top = current->previous;

			free(element);

			break;
		}

		upper = current;
		current = current->previous;
	}
}

void stackClear(Stack* stack) 
{
	StackElement* current = stack->top;

	while (current != NULL)
	{
		StackElement* temp = current;
		current = current->previous;

		free(temp);
	}

	stack->top = NULL;
	stack->first = NULL;
}

int stackIterate(Stack* stack, void* content, int(*iterateFunction)(void*, void*))
{
	assert(stack != NULL);

	StackElement* current = stack->top;

	while (current != NULL)
	{
		if (iterateFunction(current, content) == 0)
			return 0;

		current = current->previous;
	}

	return 1;
}
