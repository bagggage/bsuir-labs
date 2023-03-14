#include "exception.h"

#include <stdio.h>

void throwException(int exitCode, const char* msg) 
{
	printf("\nFatal error(%d): %s", exitCode, msg);
	exit(exitCode);
}
