#include "stdafx.h"
#include "SOMNeuron.h"


bool SOMNeuron::type;
size_t SOMNeuron::inputSize;
const float **SOMNeuron::nextInputVec;
Random SOMNeuron::random;


SOMNeuron::SOMNeuron(size_t number,
	bool type, size_t rowNum, size_t colNum,
	size_t inputSize, const float **nextInputVec,
	int tp, int bt, int le, int ri)
	: Neuron(number) , rowNum(rowNum), colNum(colNum)
{
	this->type = type;
	this->inputSize = inputSize;
	this->nextInputVec = nextInputVec;
	top = tp; bottom = bt; left = le; right = ri;

	if (number == 0)
		random = Random();
	SetWeightVec();
}

SOMNeuron::~SOMNeuron()
{
	delete[] weightVec;
}


void SOMNeuron::SetWeightVec()
{
	weightVec = new float[inputSize];
	if (this->top == 0)
		return;

	float norm = 0;
	for (size_t i = 0; i < inputSize; i++)
	{
		weightVec[i] = random.GetNextFloat(0);
		norm += weightVec[i] * weightVec[i];
	}
	norm = 1 / ((float)sqrt((double)norm));

	for (size_t i = 0; i < inputSize; i++)
		weightVec[i] *= norm;
}


uint_t SOMNeuron::Number()
{
	return uint_t(number);
}

double SOMNeuron::Potential()
{
	return potential;
}

size_t SOMNeuron::RowNum()
{
	return size_t(rowNum);
}

size_t SOMNeuron::ColNum()
{
	return size_t(colNum);
}

const float * SOMNeuron::WeighVec()
{
	return weightVec;
}

void SOMNeuron::ShowWeightVec()
{
	cout << "Weights of neuron[" << number << "]:\n";
	for (size_t i = 0; i < inputSize; i++)
		cout << weightVec[i] << ", ";
	cout << endl;
}


void SOMNeuron::ReleaseOutputs(float *synapses)
{
	synapses[number] = 0;

	if (type == 0)
	{
		for (size_t i = 0; i < inputSize; i++)
			synapses[number] +=
			nextInputVec[0][i] * weightVec[i];
	}
	else
	{
		for (size_t i = 0; i < inputSize; i++)
			synapses[number] +=
			(nextInputVec[0][i] - weightVec[i]) *
			(nextInputVec[0][i] - weightVec[i]);
		synapses[number] = sqrt(synapses[number]);
	}
}

uint_t SOMNeuron::GetDistanceFrom(size_t rowNum, size_t colNum)
{
	uint_t distance =
		(this->rowNum - rowNum) * (this->rowNum - rowNum) +
		(this->colNum - colNum) * (this->colNum - colNum);

	return distance;
}

void SOMNeuron::Update(float alpha, float tetha)
{
	for (size_t i = 0; i < inputSize; i++)
		weightVec[i] += alpha * tetha *
		(nextInputVec[0][i] - weightVec[i]);
}

void SOMNeuron::Render(HDC hdc)
{
	//create a brush and pen of the correct color
	int red = (int)(weightVec[0] * 255);
	int green = (int)(weightVec[1] * 255);
	int blue = (int)(weightVec[2] * 255);

	HBRUSH brush = CreateSolidBrush(RGB(red, green, blue));
	HPEN   pen = CreatePen(PS_SOLID, 1, RGB(red, green, blue));

	HBRUSH OldBrush = (HBRUSH)SelectObject(hdc, brush);
	HPEN   OldPen = (HPEN)SelectObject(hdc, pen);

	Rectangle(hdc, left, top, right, bottom);

	SelectObject(hdc, OldBrush);
	SelectObject(hdc, OldPen);

	DeleteObject(brush);
	DeleteObject(pen);
}

void SOMNeuron::writeWeights(std::ofstream & ofs)
{
	ofs.write(reinterpret_cast<const char*> (this->weightVec),
		sizeof(float) * this->inputSize);
}

void SOMNeuron::readWeights(std::ifstream & ifs)
{
	ifs.read(reinterpret_cast<char*> (this->weightVec),
		sizeof(float) * this->inputSize);
}
