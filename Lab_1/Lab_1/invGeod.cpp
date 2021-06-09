#pragma once
#define _USE_MATH_DEFINES
#include "invGeod.h"
#include <fstream>
#include <cassert>
#include <cmath>


invGeod::invGeod() : P1({ 0,0 }, { 0,0 }), P2({ 0, 0 }, { 0,0 }) {};

//const pair<x, y> toRect(pair<angle, angle> p)
//{
//
//}

invGeod::invGeod(
	const point p1,
	const point p2) :
	P1(p1), P2(p2) 
{};

const void invGeod::importDataFromFile(const variant var, const string filename)
{
	ifstream input(filename);
	assert(input.is_open());

	string cityname;
	degrees deg_lat, deg_long;
	minutes mins_lat, mins_long;
	size_t count = 1;
	while (input >> cityname >> deg_lat >> mins_lat >> deg_long >> mins_long)
	{
		if (count == var)
		{
			P1 = { { deg_lat, mins_lat }, { deg_long, mins_long } };
		}
		else if (count - 1 == var)
		{
			P2 = { { deg_lat, mins_lat }, { deg_long, mins_long } };
			break;
		}
		count++;
	}
}

invGeod::invGeod(const variant var, const string filename)
{
	importDataFromFile(var, filename);
}

invGeod::~invGeod() {};

const double inRad(const angle p)
{
	return (p.first + p.second / 60) * M_PI / 180.;
}

//Разность углов в радианах
const double AngleMinus(const angle x, const angle y)
{
	return inRad(x) - inRad(y);
}

const double AnglePlus(const angle x, const angle y)
{
	return inRad(x) + inRad(y);
}

const double DD(const double deg)
{
	return deg * 180 / M_PI;
}

const double invGeod::findPsi() const
{
	auto delta_longi = AngleMinus(P2.second, P1.second);
	cout << "λ12 ° . Формула (1.14) = " << fixed << DD(delta_longi) << "\n";

	double first = sin(inRad(P1.first)) * sin(inRad(P2.first));
	double second = cos(inRad(P1.first)) * cos(inRad(P2.first)) * cos(delta_longi);
	return acos(first + second);
}

//Прямой азимут в радианах
const double strtAzi(const point P1, const point P2, const double psi)
{
	const double dY = sin(AngleMinus(P2.second, P1.second)) * cos(inRad(P2.first)) / sin(psi);
	cout << "sin(A12). Формула (1.16) = " << fixed << dY << "\n";

	const double dX = (sin(inRad(P2.first)) - sin(inRad(P1.first)) * cos(psi)) / (cos(inRad(P1.first)) * sin(psi));
	cout << "cos(A12). Формула (1.17) = " << fixed << dX << "\n";
	assert(dX != 0);

	if (dX > 0 && dY > 0)
	{
		return atan(abs(dY / dX));
	}
	else if (dX < 0 && dY > 0)
	{
		return inRad({ 180,0 }) - atan(abs(dY / dX));
	}
	else if (dX < 0 && dY < 0)
	{
		return inRad({ 180,0 }) + atan(abs(dY / dX));
	}
	else if (dX > 0 && dY < 0)
	{
		return inRad({ 360,0 }) - atan(abs(dY / dX));
	}

}

//Обратный азимут в радианах
const double invAzi(const point P1, const point P2, const double psi, const double Rumba)
{
	const double dY = sin(Rumba) * cos(inRad(P1.first)) / cos(inRad(P2.first));

	const double dX = (sin(inRad(P1.first)) - sin(inRad(P2.first)) * cos(psi)) / (cos(inRad(P2.first)) * sin(psi));
	cout << "cos(A21'). Формула (1.20) = " << fixed << dX << "\n";
	assert(dX != 0);
	
	if (dX > 0 && dY > 0)
	{
		return inRad({ 360,0 }) - atan(abs(dY / dX));
	}
	else if (dX < 0 && dY > 0)
	{
		return inRad({ 360,0 }) - (inRad({ 180,0 }) - atan(abs(dY / dX)));
	}
	else if (dX < 0 && dY < 0)
	{
		return inRad({ 360,0 }) - (inRad({ 180,0 }) + atan(abs(dY / dX)));
	}
	else if (dX > 0 && dY < 0)
	{
		return inRad({ 360,0 }) - (inRad({ 360,0 }) - atan(abs(dY / dX)));
	}
}

const double setX(const point p, const int R)
{
	return R * cos(inRad(p.first)) * cos(inRad(p.second));
}

const double setY(const point p, const int R)
{
	return R * cos(inRad(p.first)) * sin(inRad(p.second));
}

const double setZ(const point p, const int R)
{
	return R * sin(inRad(p.first));
}

const double fi2(const point p, const double psi, const double A12)
{
	const double first = sin(inRad(p.first)) * cos(psi);
	const double second = cos(inRad(p.first)) * sin(psi) * cos(A12);
	return asin(first + second);
}

const double sinLambda(const double A12, const double psi, const double fi_2)
{
	return sin(A12) * sin(psi) / cos(fi_2);
}

const double cosLambda(const point p, const double psi, const double fi_2)
{
	return (cos(psi) - sin(inRad(p.first)) * sin(fi_2)) / (cos(inRad(p.first)) * cos(fi_2));
}

const double dLambda(point p1, const double A12, const double psi, const double fi_2)
{
	const double dY = sinLambda(A12, psi, fi_2);
	cout << "sin(Δλ12) . Формула (1.8) = " << dY << "\n";

	const double dX = cosLambda(p1, psi, fi_2);
	cout << "cos(Δλ12) . Формула (1.9) = " << dX << "\n";

	return atan(dY / dX);
}

const double sinA21(const point p1, const double fi_2, const double A12)
{
	return sin(A12) * cos(inRad(p1.first)) / cos(fi_2);
}

const double cosA21(const point p1, const double fi_2, const double psi)
{
	const double num = sin(inRad(p1.first)) - sin(fi_2) * cos(psi);
	const double denum = cos(fi_2) * sin(psi);
	return num / denum;
}

const double strtA21(const point P1, const double fi_2, const double psi, const double A12)
{
	const double dY = sinA21(P1, fi_2, A12);
	const double dX = cosA21(P1, fi_2, psi);
	assert(dX != 0);

	if (dX > 0 && dY > 0)
	{
		return inRad({ 360,0 }) - atan(abs(dY / dX));
	}
	else if (dX < 0 && dY > 0)
	{
		return inRad({ 360,0 }) - (inRad({ 180,0 }) - atan(abs(dY / dX)));
	}
	else if (dX < 0 && dY < 0)
	{
		return inRad({ 360,0 }) - (inRad({ 180,0 }) + atan(abs(dY / dX)));
	}
	else if (dX > 0 && dY < 0)
	{
		return inRad({ 360,0 }) - (inRad({ 360,0 }) - atan(abs(dY / dX)));
	}
}

const void invGeod::show() const
{
	const double psi = findPsi();
	cout << "ψ12 (рад) . Формула (1.13) = " << fixed << psi << "\n";

	const double A12 = strtAzi(P1, P2, psi);
	cout << "A12 ° . Формула (1.15) = " << fixed << DD(A12) << "\n";

	const double A21 = invAzi(P1, P2, psi, A12);
	cout << "A21, ° - обратный азимут. Формула (1.18) = " << fixed << DD(A21) << "\n";

	const int R = 6371000;
	const double S14 = R * psi;
	cout << "S (длина на сфере), метры. Формула (1.4) = " << S14 << "\n";

	const double
		x1 = setX(P1, R),
		y1 = setY(P1, R),
		z1 = setZ(P1, R),
		x2 = setX(P2, R),
		y2 = setY(P2, R),
		z2 = setZ(P2, R);

	cout << "x1 , метры. Формула (1.2) = " << x1 << "\n";
	cout << "y1 , метры. Формула (1.2) = " << y1 << "\n";
	cout << "z1 , метры. Формула (1.2) = " << z1 << "\n";
	cout << "x2 , метры. Формула (1.2) = " << x2 << "\n";
	cout << "y2 , метры. Формула (1.2) = " << y2 << "\n";
	cout << "z2 , метры. Формула (1.2) = " << z2 << "\n";

	const double S13 = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2) + pow(z1 - z2, 2));
	cout << "S (геоцентрич.), метры. Формула (1.3) = " << S13 << "\n";

	const double fi_2 = fi2(P1, psi, A12);
	cout << "φ2 ° . Формула (1.5) = " << DD(fi_2) << "\n";

	const double dlambda =dLambda(P1, A12, psi, fi_2);
	cout << "Δλ12 ° . Формула (1.7) = " << DD(dlambda) << "\n";

	cout << "λ2 ° . Формула (1.6) = " << DD(inRad(P1.second) + dlambda) << "\n";
}