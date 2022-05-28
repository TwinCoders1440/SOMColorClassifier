//By the name of Allah

#pragma once

#include <cstdlib>
#include <climits>
#include <stdexcept>
#include <string>
#include <exception>
#include <ctime>
#include <iomanip>

using namespace std;

class Random
{
private:
	int seed;

public:
	Random(unsigned int seed);
	Random();
	~Random();

	int GetNextInt();
	int GetNextInt(int min, int max);
	float GetNextFloat();
	float GetNextFloat(int min);
	double GetNextDouble();
	double GetNextDouble(int min);
};

