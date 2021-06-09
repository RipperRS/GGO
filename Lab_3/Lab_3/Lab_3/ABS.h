#pragma once
#include <string>
#include <vector>
using namespace std;

typedef unsigned int variant;

struct Station
{
	double X = 0;
	double Y = 0;
};

struct Measure
{
	string name = string();
	double M = 0; // горизонтальное направление
	double d = 0; // длина стороны
};

class ABS
{
	ABS();

	Station 
		A, 
		B, 
		C, 
		D;

	vector<Measure> meas;

	void calculate();

public:
	ABS(const variant, const string filename = "input.txt");
	void importData(const variant, const string filename = "input.txt");
};

