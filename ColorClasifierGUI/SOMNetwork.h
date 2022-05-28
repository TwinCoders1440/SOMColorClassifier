//By the name of Allah
#pragma once

#include "SOMNeuron.h"
#include <vector>
#include <fstream>

using namespace std;

class SOMNetwork
{
private:
	SOMNeuron **neuronPtrs;
	size_t numOfRows, numOfCols;
	float cellWidth, cellHeight;
	bool type;

	const float *nextInputVec;//Network last input
	size_t inputSize;//size of input vector
	float *synapses;//saves outputs of neurons
	size_t winnerIndex;//Netowrk last output

	Random random;
	size_t maxCycles;//maximum of cycles in training
	size_t counter;//counter for Graphical trainin
	float sigma0;//Radius of Network
	float tau;//Time constant

	ofstream ofs;

	void GenerateNetwork();
	size_t SetWinnerIndex();
	void Update(size_t counter,  float alpha0);
	bool TrainInGraphic(vector<vector<float>> &trainingData,
		float learnRate, float tolerance);
	bool TrainInNormal(vector<vector<float>> &trainingData,
		float learnRate, float tolerance);

public:
	SOMNetwork(int cx, int cy,
		size_t numOfRows, size_t numOfCols,
		size_t inputSize, bool type, size_t maxCycles = 0);
	
	SOMNetwork(std::ifstream &ifs);
	~SOMNetwork();

	size_t NumOfRows();
	size_t NumOfCols();
	size_t Size();
	const float *WeightsOf(size_t neuronNum);
	size_t InputSize();
	size_t WinnerIndex();
	float Output();
	float Sigma0();
	float Tau();

	size_t GetOutput(vector<float> &inputVec);
	void Render(HDC hdc);
	bool Train(vector<vector<float>> &trainingData,
		float learnRate, float tolerance, bool loop = true);
	bool saveModel(std::string path, HDC hdc);
};

