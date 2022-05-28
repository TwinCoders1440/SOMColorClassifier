//By the name of Allah
//This is an abstract class, representing a neuron implementation

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <exception>
#include <cmath>

using namespace std;
typedef string String;
typedef unsigned int uint_t;

class Neuron
{
private:
	void Number(const uint_t value);//set neuron number

protected: 
	uint_t number;//neuron number in layer
	double potential;//sum of the inputs to neuron

public:
	Neuron(const uint_t neuronNumber);
	~Neuron();

	virtual uint_t Number() = 0;//get neuron number
	virtual double Potential() = 0;//get potential(input) of neuron

	virtual void ReleaseOutputs(const double &input, double* synapses) = 0;
	virtual double Update(const double &learnRate, const double *frontGradients,
	const double &momentum) = 0;
};

struct SoftMax
{
	double max;
	double scale;
};

inline double GetHyperTan(const double &threshold, const double &value)
{
	if (value < -threshold)
		return -1.0;
	else if (value > threshold)
		return 1.0;
	else
		return tanh(value);
}
inline double GetSigLog(const double &threshold, const double &value)
{
	if (value < -threshold)
		return -1.0;
	else if (value > threshold)
		return 1.0;
	else
		return 1 / 1 + exp(-value);
}
inline void SetMaxScale(const vector<double> &values, SoftMax &maxScale)
{
	double max = values[0];
	for (double value : values)
		if (value > max)
			max = value;

	double scale = 0;
	for (double value : values)
		scale += exp(value - max);

	maxScale.max = max;
	maxScale.scale = scale;
}
inline void Let()
{
	cout << "\nPress any key to exit...";
	cin.get();
	cin.get();
}
inline void Fatal(string message)
{
	cout << message << endl;
	cout << "\nPress any key to exit...";
	cin.get();

	exit(-1);
}
inline void LetException()
{
	Let();
	exit(-1);
}
inline double GetDoubleRand(double fMin, double fMax)
{
	int num;

	while ((num = rand() % 100) > 100)
		num = rand() % 100;

	return 2 * (double(num / 100.00)) - 1;
}
inline float GetFloatRand(float fMin, float fMax)
{
	int num;

	while ((num = rand() % 100) > 100)
		num = rand() % 100;

	return 2 * (float(num / 100.00)) - 1;
}
inline int GetIntRand(int fMin, int fMax)
{
	int f = rand() % fMax + fMin;
	while (f > fMax || f < fMin)
		f = rand() % fMax + fMin;

	return f;
}
