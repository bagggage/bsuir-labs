#include "quicksort.h"

void swap(char** first, char** second) 
{
    char* temp = *first;

    *first = *second;
    *second = temp;
}

int partition(char** strings, int left, int right, int(*compareFunc)(char*))
{
    int pivot = compareFunc(strings[(left + right) / 2]);
    int count = 0;

    for (int i = left + 1; i <= right; i++) 
        if (compareFunc(strings[i]) <= pivot)
            count++;

    int pivotIndex = left + count;

    swap(&strings[pivotIndex], &strings[left]);

    int i = left;
    int j = right;

    while (i < pivotIndex && j > pivotIndex) 
    {
        while (compareFunc(strings[i]) <= pivot)
            i++;

        while (compareFunc(strings[j]) > pivot)
            j--;

        if (i < pivotIndex && j > pivotIndex)
            swap(&strings[i++], &strings[j--]);
    }

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
