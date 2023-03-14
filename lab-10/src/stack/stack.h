#pragma once

#include <stdlib.h>

#define STACK_ELEMENT_BODY StackElement* previous;

typedef struct StackElement
{
	struct StackElement* previous;
} StackElement;

typedef struct Stack 
{
	StackElement* first;
	StackElement* top;
} Stack;

Stack stackInit();

void stackPush(Stack* stack, void* stackElement, size_t elementSize);
void stackPop(Stack* stack);
void stackRemove(Stack* stack, StackElement* element);
void stackClear(Stack* stack);

int stackIterate(Stack* stack, void* content, int(*iterateFunction)(void*, void*));
