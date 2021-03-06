#pragma once

#include "resource.h"

vector<vector<float> > trainingSet;
//-------------------------- CreateDataSet -------------------------------
//
//------------------------------------------------------------------------
void CreateDataSet()
{

#ifndef RANDOM_TRAINING_SETS

	//create a data set
	vector<float> red, green, blue, yellow, orange, purple, dk_green, dk_blue;
	//vector<float> white, black;

	red.push_back(1);
	red.push_back(0);
	red.push_back(0);

	green.push_back(0);
	green.push_back(1);
	green.push_back(0);

	dk_green.push_back(0);
	dk_green.push_back(0.5);
	dk_green.push_back(0.25);

	blue.push_back(0);
	blue.push_back(0);
	blue.push_back(1);

	dk_blue.push_back(0);
	dk_blue.push_back(0);
	dk_blue.push_back(0.5);

	yellow.push_back(1);
	yellow.push_back(1);
	yellow.push_back(0.2);

	orange.push_back(1);
	orange.push_back(0.4);
	orange.push_back(0.25);

	purple.push_back(1);
	purple.push_back(0);
	purple.push_back(1);

	trainingSet.push_back(red);
	trainingSet.push_back(green);
	trainingSet.push_back(blue);
	trainingSet.push_back(yellow);
	trainingSet.push_back(orange);
	trainingSet.push_back(purple);
	trainingSet.push_back(dk_green);
	trainingSet.push_back(dk_blue);
	
	/*
	white.push_back(1.0);
	white.push_back(1.0);
	white.push_back(1.0);

	black.push_back(1.0 * 0.3);
	black.push_back(1.0 * 0.3);
	black.push_back(1.0 * 0.3);

	trainingSet.push_back(black);
	trainingSet.push_back(white);
	*/

#else

	//choose a random number of training sets
	int NumSets = RandInt(constMinNumTrainingSets, constMaxNumTrainingSets);

	for (int s = 0; s<NumSets; ++s)
	{

		vector<double> set;

		set.push_back(RandFloat());
		set.push_back(RandFloat());
		set.push_back(RandFloat());

		m_TrainingSet.push_back(set);
	}

#endif

}