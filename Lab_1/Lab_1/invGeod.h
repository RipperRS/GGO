#pragma once
#include <iostream>
using namespace std;

typedef int variant;
typedef int degrees;
typedef double minutes;
typedef pair<degrees, minutes> angle, latitude, longitude;
typedef pair<latitude, longitude> point;

class invGeod
{
	point P1, P2;

protected:
	const double findPsi() const;

public:
	invGeod();
	~invGeod();
	invGeod(
		const point,
		const point);

	const void importDataFromFile(const variant, const string);
	invGeod(const variant, const string = "input.txt");
	const void show() const;
};