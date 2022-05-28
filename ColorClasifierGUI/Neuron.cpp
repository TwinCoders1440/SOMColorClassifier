//By the name of Allah

#include "stdafx.h"
#include "Neuron.h"


Neuron::Neuron(const uint_t neuronNumber)
{
	Number(neuronNumber);
}
Neuron::~Neuron()
{
}

//...........................................................Properties

void Neuron::Number(const uint_t value)
{
	if (value >= 0)
		number = value;
	else
		throw range_error("In Number(value), value "\
			"must be greater than -1\nvalue = " + to_string(value));
}
