#pragma once

#include <string>
#include <map>
#include <vector>

using namespace std;

typedef int variant;
typedef int degrees, minutes;

class Sphere
{
	struct Angle
	{
		degrees degrees = 0;
		minutes minutes = 0;
	};
	struct Point
	{
		Angle latitude = { 0,0 }; // широта
		Angle longitude = { 0,0 }; // долгота
	};


	Point P1, P2;

protected:
	
	const double Rad(const Angle angle) const;
	const double Rad(const double grad) const;
	const double Grad(const double rad) const;
	const double set_N(const double phi) const;
	const double set_M(const double phi) const;
	const double set_Eta_2(const double phi) const;

	const vector<double> set_f();
	const double Azimuth(const double dX, const double dY) const;
	const double setLambda_2(const size_t i) const;
	const double setPhi_2(const size_t i) const;

	const double
		a = 6378137.0,
		b = 6356863.019,
		f = 1.0 / 298.257223563,
		_1_f = 298.257223563;

	double
		delta_phi,
		delta_lambda,
		phi;

	double e;

	double N;
	double M;

	double t;

	double eta_2;
	double V_2;
	
	double
		s_sina,
		s_cosa,
		delta_a;

	double
		f1,
		f2,
		f3,
		f4,
		f5,
		f6,
		f7,
		f8;

	double S;

	double r;

	double alpha;

	double a1, a2;

	double phi2_1, lambda2_1;

	double N1,N2;

	double
		x1, y1, z1,
		x2, y2, z2;

	double lambda_1, lambda_2;
	
	double epsilon;

	double _b;
	double p1, p2;
	double q1, q2;
	double h_1, h_2;
	double phi_1, phi_2;
	double S_geocent;

	vector<vector<pair<const string, const double>>> iterations;

public:

	Sphere();
	Sphere(const variant variant, const string filename = "input.txt");

	void calculation();
	void importDataFromFile(const variant variant, const string filename);
	void calcIterations(const size_t iter = 3);
	void showAnswers() const;
};

