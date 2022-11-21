long long randomState;
const long long randomMax = 4294967296;

void setRandomSeed(long long seed) 
{
	randomState = seed;
}

unsigned int random()
{
	randomState = 214013 * randomState + 2531011;
	randomState ^= randomState >> 15;

	return (unsigned int)(randomState % randomMax);
}

int randomRange(int min, int max)
{
	return random() % (max - min) + min;
}