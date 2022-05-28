#include "stdafx.h"
#include "Random.h"


Random::Random(unsigned int seed)
{
	srand(seed);
}

Random::Random()
{
}

Random::~Random()
{
}


int Random::GetNextInt()
{
	int r = rand() % INT_MAX + INT_MIN;
	while (r > INT_MAX || r < INT_MIN)
		r = rand() % INT_MAX + INT_MIN;

	return r;
}

int Random::GetNextInt(int min, int max)
{
	int r = (rand() % (max + 1)) + min;
	while (r > max || r < min)
		r = (rand() % (max + 1)) + min;

	return r;
}

float Random::GetNextFloat()
{
	int num;

	while ((num = rand() % 101) >= 100)
		num = rand() % 101;

	return 2 * ((float)num / (float)100.00) - 1;
}

float Random::GetNextFloat(int min)
{
	int num;

	while ((num = rand() % 101) >= 100)
		num = rand() % 101;

	if (min == -1)
		return 2 * ((float)num / (float)100.00) + min;
	else if (min == 0)
		return ((float)num / (float)100.00);
	else
		throw runtime_error("Random::GetNextFloat(int min):\n"\
			"min must be 0 ro -1.\nmin = " + to_string(min));
}

double Random::GetNextDouble()
{
	int num;

	while ((num = rand() % 101) >= 100)
		num = rand() % 101;

	return 2 * ((double)num / 100.00) - 1;
}

double Random::GetNextDouble(int min)
{
	int num;

	while ((num = rand() % 101) >= 100)
		num = rand() % 101;

	if (min == -1)
		return 2 * ((double)num / 100.00) + min;
	else if (min == 0)
		return ((double)num / 100.00);
	else
		throw runtime_error("Random::GetNextDouble(int min):\n"\
			"min must be 0 ro -1.\nmin = " + to_string(min));
}
