#include <iostream>
#include <fstream>

#include "Read.h"

float** Read::readData()
{
	std::ifstream is("ExchangeRates.csv");

	std::vector<float> rates1;
	std::vector<float> rates2;
	std::vector<float> rates3;
	std::vector<float> rates4;

	std::string date;
	float rate1, rate2, rate3, rate4;
	char comma;

	while (is >> date >> rate1 >> comma >> rate2 >> comma >> rate3 >> comma >> rate4)
	{
		rates1.push_back(rate1);
		rates2.push_back(rate2);
		rates3.push_back(rate3);
		rates4.push_back(rate4);
	}

	int dataSize = rates1.size();

	std::vector<float> v[4];

	v[0] = rates1;
	v[1] = rates2;
	v[2] = rates3;
	v[3] = rates4;

	float** data = 0;
	data = new float*[4];

	for(int i = 0; i < 4; i++)
	{
		data[i] = new float[dataSize];

		for(int j = 0; j < dataSize; j++)
		{
			//reverse order of the data so it may be shown chronologically
			data[i][(dataSize - 1) - j] = v[i].at(j);
		}
	}

	return data;
}
