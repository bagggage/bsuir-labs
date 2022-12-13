#include "quicksort.h"

void swap(char** first, char** second) 
{
    char* temp = *first;

    *first = *second;
    *second = temp;
}

int partition(char** strings, int left, int right, int(*compareFunc)(char*))
{
    int pivot = compareFunc(strings[right]);
    int pivotIndex = left;

    for (int i = left; i < right; i++)
    {
        if (compareFunc(strings[i]) <= pivot)
        {
            swap(&strings[i], &strings[pivotIndex]);
            pivotIndex++;
        }
    }

    swap(&strings[pivotIndex], &strings[right]);

    return pivotIndex;
}

void quickSort(char** strings, int left, int right, int(*compareFunc)(char*))
{
    if (left >= right)
        return;

    int p = partition(strings, left, right, compareFunc);

    quickSort(strings, left, p - 1, compareFunc);
    quickSort(strings, p + 1, right, compareFunc);
}
