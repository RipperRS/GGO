#pragma once
#include "ABS.h"
#include <fstream>
#include <cassert>
#include <iostream>
using namespace std;


ABS::ABS() : A(), B(), C(), D() {};

void ABS::importData(const variant var, const string filename)
{
	const string directory = "src\\" + filename;
	ifstream file(directory);
	assert(file.is_open());

	meas.resize(13);

	uintptr_t line_counter = 1;
	string name, first, second;
	while (file >> name >> first >> second)
	{
		switch (line_counter)
		{
		case 1:
			A = { stod(first), stod(second) };
			break;
		case 2:
			B = { stod(first), stod(second) };
			break;
		case 3:
			C = { stod(first), stod(second) };
			break;
		case 4:
			D = { stod(first), stod(second) };
			break;
		case 5:
			meas.at(line_counter - 5).name = name;
			meas.at(line_counter - 5).d = stod(first);
			break;
		default:
			meas.at(line_counter - 5).name = name;
			meas.at(line_counter - 5).M = stod(first);
			break;
		}
	
		line_counter++;
	}
	file.close();
}

void ABS::calculate()
{

}

ABS::ABS(const variant var, const string filename)
{
	importData(var, filename);
	calculate();

}



