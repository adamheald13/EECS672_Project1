#include <iostream>
#include <fstream>

Read::Read() {
	std::cout << "read creation..." << std::endl;
}

Read::~Read() {
	std::cout << "read destroy..." << std::endl;
}

void Read::readData()
{
	std::ifstream is("ExchangeRates.csv");

	std::string date;
	float rate1, rate2, rate3, rate4;
	char comma;
	std::cout << "First five lines:\n";
	int nLines = 0;
	while (is >> date >> rate1 >> comma >> rate2 >> comma >> rate3 >> comma >> rate4)
	{
		if (++nLines <= 5)
			std::cout << date << ' ' << rate1 << ' ' << rate2 << ' ' << rate3 << ' ' << rate4 << '\n';
	}
	std::cout << "\nLast line:\n";
	std::cout << date << ' ' << rate1 << ' ' << rate2 << ' ' << rate3 << ' ' << rate4 << '\n';
}
