//By the name of Allah
#pragma once

#include "Neuron.h"
#include "Random.h"
#include <Windows.h>
#include <fstream>


class SOMNeuron :
	public Neuron
{
private:
	static bool type;//0 potential, 1 distancial

	size_t rowNum;//y of the neuron in lathic
	size_t colNum;// x of the neuron in lathic
	int top, bottom, left, right;

	float *weightVec;
	void SetWeightVec();
	static size_t inputSize;
	static Random random;

	static const float **nextInputVec;

	virtual void ReleaseOutputs(const double &input,
		double* synapses) {return;}
	virtual double Update(const double &learnRate,
		const double *frontGradients,
		const double &momentum) {return 0.0;}

public:
	SOMNeuron(size_t number,
		bool type, size_t rowNum, size_t colNum,
		size_t inputSize, const float **nextInputVec,
		int tp, int bt, int le, int ri);
	~SOMNeuron();

	unsigned int Number() override;//get neuron number
	double Potential() override;//get potential(input) of neuron
	size_t RowNum();
	size_t ColNum();

	const float *WeighVec();
	void ShowWeightVec();

	void ReleaseOutputs(float* synapses);
	uint_t GetDistanceFrom(size_t rowNum, size_t colNum);
	void Update(float alpha, float tetha);
	void Render(HDC hdc);

	void writeWeights(std::ofstream &ofs);
	void readWeights(std::ifstream &ifs);
};
