#include "stdafx.h"
#include "SOMNetwork.h"

SOMNetwork::SOMNetwork(int cx, int cy,
	size_t numOfRows, size_t numOfCols,
	size_t inputSize, bool type, size_t maxCycles)
{
	this->numOfRows = numOfRows;
	this->numOfCols = numOfCols;
	this->type = type;
	this->inputSize = inputSize;
	this->neuronPtrs = new SOMNeuron*[numOfRows * numOfCols];
	this->synapses = new float[numOfRows * numOfCols];
	this->maxCycles = maxCycles;
	this->counter = 0;
	this->cellWidth = (float)cx / (float)numOfCols;
	this->cellHeight = (float)cy / (float)numOfRows;

	this->sigma0 = (numOfRows > numOfCols) ?
		(float)numOfRows : (float)numOfCols;
	this->tau = (float)maxCycles / log(sigma0);

	GenerateNetwork();
}

SOMNetwork::SOMNetwork(std::ifstream &ifs)
{
	ifs >> this->type >> this->numOfRows >> this->numOfCols >>
		this->inputSize;
	ifs.get();//skip \n
	this->neuronPtrs = new SOMNeuron*[numOfRows * numOfCols];
	this->synapses = new float[numOfRows * numOfCols];

	for (size_t i = 0; i < numOfRows; i++)
		for (size_t j = 0; j < numOfCols; j++)
		{
			size_t index = i * numOfCols + j;
			neuronPtrs[index] = new SOMNeuron(i * numOfCols + j,
					type, i, j, inputSize, &nextInputVec,
					0, 0, 0, 0);
			neuronPtrs[index]->readWeights(ifs);
			if (ifs.fail()) {
				throw std::string("SOMNetwork::SOMNetwork():\n"\
					"can't read weights of ") + std::to_string(index) +
					" neuron from file stream.\n";
			}
		}

	winnerIndex = 0;
}

SOMNetwork::~SOMNetwork()
{
	for (size_t i = 0; i < numOfRows * numOfCols; i++)
		delete neuronPtrs[i];
	delete[] neuronPtrs;
	
	delete[] synapses;
}


void SOMNetwork::GenerateNetwork()
{
	for (size_t i = 0; i < numOfRows; i++)
		for (size_t j = 0; j < numOfCols; j++)
		{
			neuronPtrs[i * numOfCols + j] =
				new SOMNeuron(i * numOfCols + j,
					type, i, j, inputSize,
					&nextInputVec,
					(int)(i * cellHeight),
					(int)((i + 1) * cellHeight),
					(int)(j * cellWidth),
					(int)((j + 1) * cellWidth));
		}

	winnerIndex = 0;
	random = Random();
}

size_t SOMNetwork::SetWinnerIndex()
{
	float max = synapses[0];
	float min = synapses[0];
	if (type == false)
	{
		for (size_t i = 0; i < Size(); i++)
			if (synapses[i] > max)
			{
				max = synapses[i];
				winnerIndex = i;
			}
	}//end if of potential output
	else
	{
		for (size_t i = 0; i < Size(); i++)
			if (synapses[i] < min)
			{
				min = synapses[i];
				winnerIndex = i;
			}
	}//end if of distantial output

	return winnerIndex;
}

void SOMNetwork::Update(size_t counter, float alpha0)
{
	float sigma, alpha, theta;
	size_t winRow = neuronPtrs[winnerIndex]->RowNum()
		, winCol = neuronPtrs[winnerIndex]->ColNum();

	sigma = sigma0 * exp(-1 * (int)counter / tau);
	sigma *= sigma;
	alpha = alpha0 * exp(-1 * (int)counter / tau);

	for (size_t i = 0; i < Size(); i++)
	{
		size_t dis = neuronPtrs[i]->GetDistanceFrom(winRow, winCol);
		if (dis > sigma)
			continue;

		theta = exp(-1 * (dis / (2 * sigma)));
		neuronPtrs[i]->Update(alpha, theta);
	}
}

bool SOMNetwork::TrainInGraphic(vector<vector<float>>& trainingData,
	float learnRate, float tolerance)
{
	int randomNum;

	counter++;
	size_t ptrnCounter = 0;
	float averDisPerCycle = 0;
	for (size_t j = 0; j < trainingData.size(); j++)
	{
		randomNum = random.GetNextInt(0, trainingData.size() - 1);
		nextInputVec = trainingData[j].data();
		ptrnCounter++;

		GetOutput(trainingData[j]);
		averDisPerCycle += synapses[winnerIndex];
		ofs << counter << '\t' << j << '\t' << winnerIndex << endl;
		Update(counter, learnRate);
	}//end traning data set

	if ((averDisPerCycle / ptrnCounter) < tolerance
		|| counter > maxCycles)
		return false;
	else
		return true;
}

bool SOMNetwork::TrainInNormal(vector<vector<float>>& trainingData,
	float learnRate, float tolerance)
{
	if (maxCycles == 0)
		Fatal("SOMNetwork::Train():\n"\
			"maxCycles must be bigger than 0.");

	ofstream ofs;
	ofs.open("output.txt");
	int randomNum;
	for (size_t i = 0; i < maxCycles; i++)
	{
		size_t ptrnCounter = 0;
		float averDisPerCycle = 0;
		for (size_t j = 0; j < trainingData.size(); j++)
		{
			randomNum = random.GetNextInt(0, trainingData.size() - 1);
			ptrnCounter++;

			GetOutput(trainingData[j]);
			averDisPerCycle += synapses[winnerIndex];
			ofs << i << '\t' << j << '\t' << winnerIndex << endl;
			Update(i, learnRate);
		}//end traning data set

		if ((averDisPerCycle / ptrnCounter) < tolerance)
			break;
	}//end cycle
	ofs.close();
	return false;
}


size_t SOMNetwork::NumOfRows()
{
	return size_t(numOfRows);
}

size_t SOMNetwork::NumOfCols()
{
	return size_t(numOfCols);
}

size_t SOMNetwork::Size()
{
	return size_t(numOfRows * numOfCols);
}

const float * SOMNetwork::WeightsOf(size_t neuronNum)
{
	if (neuronNum < Size())
		return neuronPtrs[neuronNum]->WeighVec();

	throw out_of_range("SOMNetwork::WeightsOf(value):\n"\
		"value must be smaller than network size.\nvalue = " +
		to_string(neuronNum));
}

size_t SOMNetwork::InputSize()
{
	return size_t(inputSize);
}

size_t SOMNetwork::WinnerIndex()
{
	return size_t(winnerIndex);
}

float SOMNetwork::Output()
{
	return synapses[winnerIndex];
}


float SOMNetwork::Sigma0()
{
	return sigma0;
}

float SOMNetwork::Tau()
{
	return tau;
}


size_t SOMNetwork::GetOutput(vector<float> &inputVec)
{
	if (inputVec.size() != inputSize)
		throw out_of_range("SOMNetwork::GetOutput(value):\n"\
			"value size must be equal to inputSize of network.\n"\
			"value size = " + to_string(inputVec.size()));

	nextInputVec = inputVec.data();

	for (size_t i = 0; i < Size(); i++)
		neuronPtrs[i]->ReleaseOutputs(synapses);

	return SetWinnerIndex();

}

void SOMNetwork::Render(HDC hdc)
{
	for (size_t i = 0; i < Size(); i++)
		neuronPtrs[i]->Render(hdc);

	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(0, 0, 255));

	wstring s = L"Iteration: " + to_wstring(counter);
	TextOut(hdc, 20, 300, s.c_str(), s.size());

	s = L"Press 'R' to retrain";
	TextOut(hdc, 260, 300, s.c_str(), s.size());
}

bool SOMNetwork::Train(vector<vector<float>> &trainingData,
	float learnRate, float tolerance, bool loop)
{
	if (loop == true)
		return TrainInNormal(trainingData, learnRate, tolerance);

	if (counter == 0)
	{
		if (maxCycles == 0)
			Fatal("SOMNetwork::Train():\n"\
				"maxCycles must be bigger than 0.");

		ofs.open("output.txt");
	}

	if (!TrainInGraphic(trainingData, learnRate, tolerance))
	{
		ofs.close();
		return false;
	}
	else
		return true;
}

bool SOMNetwork::saveModel(std::string path, HDC hdc)
{
	ofstream modelFile;
	modelFile.open(path, std::ios::out | std::ios::binary);
	if (!modelFile) {
		wstring s = L"Error when saving model in " + std::wstring(path.begin(), path.end());
		TextOut(hdc, 20, 200, s.c_str(), s.size());
		return false;
	}

	modelFile << this->type << std::endl;
	modelFile << this->numOfRows << "\t" << this->numOfCols << "\n";
	modelFile << this->inputSize << std::endl << std::endl;
	for (int i = 0; i < this->Size(); ++i) {
		this->neuronPtrs[i]->writeWeights(modelFile);

		if (modelFile.fail()) {
			wstring s = L"Error when saving weights of neuron " + to_wstring(i);
			TextOut(hdc, 20, 200, s.c_str(), s.size());
			return false;
		}
	}
	wstring s = L"model sucssesfuly saved in " + std::wstring(path.begin(), path.end());
	TextOut(hdc, 20, 200, s.c_str(), s.size());

	return true;
}
