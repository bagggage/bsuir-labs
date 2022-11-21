#include "algorithm.h"

void swap(int* firstElementPtr, int* secondElementPtr)
{
	int tempValue = *firstElementPtr;

	*firstElementPtr = *secondElementPtr;
	*secondElementPtr = tempValue;
}

void swapRowsInMatrix(int* firstRow, int* secondRow, unsigned int columnsCount)
{
	for (int i = 0; i < columnsCount; i++) 
	{
		int tempValue = firstRow[i];

		firstRow[i] = secondRow[i];
		secondRow[i] = tempValue;
	}
}

int getCountOfEvenElementsInArray(int* array, unsigned int arraySize) 
{
	int count = 0;

	for (int i = 0; i < arraySize; i++)
		if (array[i] % 2 == 0)
			count++;

	return count;
}

void quickSort(int* array, int left, int right, int step)
{
	int leftHolder = left;
	int rightHolder = right;
	int pivot;

	if (step > 0)
	{
		pivot = array[left + (((right - left + 1) / step) / 2) * step];

		do
		{
			while (array[leftHolder] < pivot)
				leftHolder += step;

			while (array[rightHolder] > pivot)
				rightHolder -= step;

			if (leftHolder <= rightHolder)
			{
				if (array[leftHolder] > array[rightHolder])
					swap(&array[leftHolder], &array[rightHolder]);

				leftHolder += step;
				rightHolder -= step;
			}

		} while (leftHolder <= rightHolder);
	}

	else
	{
		pivot = array[left + (((right - left + 1) / -step) / 2) * -step];

		do
		{
			while (array[leftHolder] > pivot)
				leftHolder += -step;

			while (array[rightHolder] < pivot)
				rightHolder -= -step;

			if (leftHolder <= rightHolder)
			{
				if (array[leftHolder] < array[rightHolder])
					swap(&array[leftHolder], &array[rightHolder]);

				leftHolder += -step;
				rightHolder -= -step;
			}

		} while (leftHolder <= rightHolder);
	}

	if (leftHolder < right)
		quickSort(array, leftHolder, right, step);

	if (left < rightHolder)
		return quickSort(array, left, rightHolder, step);
}

void bubleSort(int* array, unsigned int arraySize, int step)
{
	for (int i = step - 1; i < arraySize - 1; i += step)
		for (int j = arraySize - 1; j > i; j -= step)
			if (array[j - step] > array[j])
			{
				int tempValue = array[j - step];

				array[j - step] = array[j];
				array[j] = tempValue;
			}
}