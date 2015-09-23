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
		std::cout << "adding: " << rate1 << " to vector." << std::endl;
		rates1.push_back(rate1);

		std::cout << "adding: " << rate2 << " to vector." << std::endl;
		rates2.push_back(rate2);

		std::cout << "adding: " << rate3 << " to vector." << std::endl;
		rates3.push_back(rate3);

		std::cout << "adding: " << rate4 << " to vector." << std::endl;
		rates4.push_back(rate4);
		//std::cout << date << ' ' << rate1 << ' ' << rate2 << ' ' << rate3 << ' ' << rate4 << '\n';
	}

	int dataSize = rates1.size();

	std::vector<float> v[4];

	v[0] = rates1;
	v[1] = rates2;
	v[2] = rates3;
	v[3] = rates4;

	float data[4][dataSize];

	for(int i = 0; i < 4; i++) {
		for(int j = dataSize - 1; j >= 0; j++) {
			data[i][j] = v[i].at(j);
		}
	}

	return data;
}
