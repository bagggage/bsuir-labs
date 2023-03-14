#include <stdio.h>

#include "compression/decompression.h"
#include "compression/compression.h"

int main() 
{
	FILE* file;

	int result = fopen_s(&file, "test.txt", "r");

	if (result != 0)
	{
		perror("File open error");
		return 1;
	}

	compress(file, "testCompressed.txt");

	fclose(file);

	return 0;
}
