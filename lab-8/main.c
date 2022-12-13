#include <stdio.h>
#include <stdlib.h>

#include "string.h"
#include "quicksort.h"

int main(int argc, char** argv)
{
	char** strings = malloc((argc - 1) * sizeof(char*));

	printf("Input:\n");

	for (int i = 1; i < argc; i++)
	{
		strings[i - 1] = initString(argv[i]);

		printf("\t%s\n", strings[i - 1]);
	}

	quickSort(strings, 0, argc - 1 - 1, &countWordsInString);

	printf("\nSorted array of strings:\n");

	for (int i = 0; i < argc - 1; i++)
	{
		printf("\t%s\n", strings[i]);
		free(strings[i]);
	}

	free(strings);

	return 0;
}