#pragma once
#define _USE_MATH_DEFINES


#define delta_phi(iteration) iterations[iteration].at(0).second
#define delta_lambda(iteration) iterations[iteration].at(1).second
#define phi(iteration) iterations[iteration].at(2).second
#define N(iteration) iterations[iteration].at(3).second
#define M(iteration) iterations[iteration].at(4).second
#define eta_2(iteration) iterations[iteration].at(5).second
#define V_2(iteration) iterations[iteration].at(6).second
#define t(iteration) iterations[iteration].at(7).second
#define f1(iteration) iterations[iteration].at(8).second
#define f2(iteration) iterations[iteration].at(9).second
#define f3(iteration) iterations[iteration].at(10).second
#define f4(iteration) iterations[iteration].at(11).second
#define f5(iteration) iterations[iteration].at(12).second
#define f6(iteration) iterations[iteration].at(13).second
#define f7(iteration) iterations[iteration].at(14).second
#define f8(iteration) iterations[iteration].at(15).second
#define delta_a(iteration) iterations[iteration].at(16).second
#define a(iteration) iterations[iteration].at(17).second
#define a2(iteration) iterations[iteration].at(18).second

#include "sphere.h"

#include <iostream>
#include <fstream>
#include <cmath>
#include <cassert>
#include <iomanip>

using namespace std;

Sphere::Sphere() : P1(), P2() 
{
	calculation();
};

void Sphere::importDataFromFile(const variant variant, const string filename)
{
	ifstream file("src\\" + filename);
	if (!file.is_open())
	{
		cerr << "src\\" + filename << " is not exist\n";
	}

	unsigned int number;
	string cityname;
	degrees lat_deg, long_deg;
	minutes lat_min, long_min;
	_Atomic_counter_t var = 1;

	while (file >> number >> cityname >> lat_deg >> lat_min >> long_deg >> long_min)
	{
		if (var == variant)
		{
			P1.latitude = { lat_deg, lat_min };
			P1.longitude = { long_deg, long_min };
		}
		if (var == variant + 1)
		{
			P2.latitude = { lat_deg, lat_min };
			P2.longitude = { long_deg, long_min };
			break;
		}
		var++;
	}
}

Sphere::Sphere(const variant variant, const string filename)
{
	importDataFromFile(variant, filename);
	calculation();
}

const double Sphere::Rad(const Angle angle) const
{
	return (angle.degrees + angle.minutes / 60.0) * M_PI / 180;
}

const double Sphere::Rad(const double grad) const
{
	return grad * M_PI / 180;
}

const double Sphere::Grad(const double rad) const
{
	return rad * 180 / M_PI;
}

const double Sphere::set_N(const double phi) const
{
	return a / sqrt(1 - e * pow(sin(phi), 2));
}

const double Sphere::set_M(const double phi) const
{
	return (a * (1 - e)) / pow(1 - e * pow(sin(phi), 2), 3.0 / 2);
}

const double Sphere::set_Eta_2(const double phi) const
{
	return (e * pow(cos(phi), 2)) / (1 - e);
}

// Возвращает азимут в радианах
const double Sphere::Azimuth(const double dX, const double dY) const
{
	if (dX > 0 && dY > 0)
	{
		return r;
	}
	else if (dX < 0 && dY > 0)
	{
		return Rad(180 - Grad(r));
	}
	else if (dX < 0 && dY < 0)
	{
		return Rad(180 + Grad(r));
	}
	else if (dX > 0 && dY < 0)
	{
		return Rad(360 - Grad(r));
	}
}

const double set_f4(const double eta_2, const double t, const double V_2)
{
	return (1 + eta_2 - 9 * eta_2 * t * t) / (24 * V_2 * V_2);
}

const double set_f6(const double eta_2, const double t, const double V_2)
{
	return (eta_2 * (1 - t * t)) / (8 * V_2 * V_2);
}

const double set_f8(const double eta_2, const double V_2)
{
	return (3 + 8 * eta_2) / (24 * V_2 * V_2);
}

const double Delta_a(const double delta_lambda, const double phi, const double f7, const double f8, const double delta_phi)
{
	return delta_lambda * sin(phi) * (1 + f7 * pow(delta_lambda * cos(phi), 2) + f8 * delta_phi * delta_phi);
}

const double Sphere::setLambda_2(const size_t i) const
{
	return Rad(P1.longitude) + f2(i) * S * sin(a(i)) / cos(phi(i)) *
		(1 + f3(i) * pow(delta_lambda(i) * sin(phi(i)), 2) - f4(i) * delta_phi(i) * delta_phi(i));
}

const double Sphere::setPhi_2(const size_t i) const
{
	return Rad(P1.latitude) + f1(i) * S * cos(a(i)) / cos(delta_a(i) / 2) *
		(1 - f5(i) * pow(delta_lambda(i) * cos(phi(i)), 2) - f6(i) * delta_phi(i) * delta_phi(i));
}

void Sphere::calcIterations(const size_t iter)
{
	const double
		P1_latitude_rad = Rad(P1.latitude), P1_longitude_rad = Rad(P1.longitude),
		P2_latitude_rad = Rad(P2.latitude), P2_longitude_rad = Rad(P2.longitude);

	double
		N = set_N(P1_latitude_rad),
		M = set_M(P1_latitude_rad);

	double
		phi_2 = P1_latitude_rad + (S * cos(a1)) / M,
		lambda_2 = P1_longitude_rad + (S * sin(a1)) / (N * cos(P1_latitude_rad));

	phi2_1 = phi_2;
	lambda2_1 = lambda_2;

	double
		a1 = this->a1,
		a2 = this->a2;

	for (size_t i = 0; i < iter; ++i)
	{
		iterations.push_back(vector<pair<const string, const double>>());                                 // Порядковый номер
		iterations.back().push_back({ "delta phi", phi_2 - P1_latitude_rad });                // 0
		iterations.back().push_back({ "delta lambda", lambda_2 - P1_longitude_rad });         // 1
		iterations.back().push_back({ "phi", (phi_2 + P1_latitude_rad) / 2 });                // 2
		iterations.back().push_back({ "N", set_N(phi(i)) });          // 3
		iterations.back().push_back({ "M", set_M(phi(i)) });          // 4
		iterations.back().push_back({ "eta_2", set_Eta_2(phi(i)) });    // 5  
		iterations.back().push_back({ "V_2", 1 + eta_2(i) });          // 6
		iterations.back().push_back({ "t",	tan(phi(i)) });           // 7

		iterations.back().push_back({ "f1", 1 / M(i) });            // 8
		iterations.back().push_back({ "f2", 1 / N(i) });            // 9
		iterations.back().push_back({ "f3", 1.0 / 24 });            // 10
		iterations.back().push_back({ "f4", set_f4(eta_2(i), t(i), V_2(i)) }); // 11
		iterations.back().push_back({ "f5", (1 - 2 * eta_2(i)) / 24 }); // 12
		iterations.back().push_back({ "f6", set_f6(eta_2(i), t(i) ,V_2(i)) }); // 13
		iterations.back().push_back({ "f7", (1 + eta_2(i)) / 12 });     // 14
		iterations.back().push_back({ "f8", set_f8(eta_2(i), V_2(i)) }); // 15

		iterations.back().push_back({ "delta a", Delta_a(delta_lambda(i), phi(i), f7(i), f8(i), delta_phi(i)) }); // 16
		iterations.back().push_back({ "a", a1 + delta_a(i) / 2 });       // 17
		iterations.back().push_back({ "a2", a(i) + delta_a(i) / 2 + M_PI }); // 18
		iterations.back().push_back({ "lambda", setLambda_2(i) }); // 19
		iterations.back().push_back({ "phi", setPhi_2(i) }); // 20

		lambda_2 = setLambda_2(i);
		phi_2 = setPhi_2(i);
	}
}

void Sphere::calculation()
{
	const double
		P1_latitude_rad = Rad(P1.latitude), P1_longitude_rad = Rad(P1.longitude),
		P2_latitude_rad = Rad(P2.latitude), P2_longitude_rad = Rad(P2.longitude);

	delta_phi = P2_latitude_rad - P1_latitude_rad;
	delta_lambda = P2_longitude_rad - P1_longitude_rad;
	phi = (P1_latitude_rad + P2_latitude_rad) / 2;

	e = 2 * f - f * f;

	N = a / sqrt(1 - e * pow(sin(phi), 2));
	M = (a * (1 - e)) / pow(1 - e * pow(sin(phi), 2), 3.0 / 2);

	t = tan(phi);

	eta_2 = (e * pow(cos(phi), 2)) / (1 - e);
	V_2 = 1 + eta_2;

	f1 = 1 / M;
	f2 = 1 / N;
	f3 = 1.0 / 24;
	f4 = (1 + eta_2 - 9 * eta_2 * t * t) / (24 * V_2 * V_2);
	f5 = (1 - 2 * eta_2) / 24;
	f6 = (eta_2 * (1 - t * t)) / (8 * V_2 * V_2);
	f7 = V_2 / 12;
	f8 = (3 + 8 * eta_2) / (24 * V_2 * V_2);

	s_sina = (1 / f2) * delta_lambda * cos(phi) * (1 - f3 * pow(delta_lambda * sin(phi), 2) + f4 * delta_phi * delta_phi);
	s_cosa = (1 / f1) * delta_phi * cos(delta_lambda / 2) * (1 + f5 * pow(delta_lambda * cos(phi), 2) + f6 * delta_phi * delta_phi);
	delta_a = delta_lambda * sin(phi) * (1 + f7 * pow(delta_lambda * cos(phi), 2) + f8 * delta_phi * delta_phi);

	S = sqrt(s_sina * s_sina + s_cosa * s_cosa);

	assert(s_sina != 0 && s_cosa != 0);
	r = atan(abs(s_sina / s_cosa));

	alpha = Azimuth(s_cosa, s_sina);

	cout << setprecision(6) <<fixed << Grad(alpha);
	
	a1 = alpha - delta_a / 2;
	a2 = alpha + delta_a / 2 + M_PI;

	calcIterations();

	N1 = a / sqrt(1 - e * pow(sin(P1_latitude_rad), 2));
	x1 = N1 * cos(P1_latitude_rad) * cos(P1_longitude_rad);
	assert(x1 != 0);
	y1 = N1 * cos(P1_latitude_rad) * sin(P1_longitude_rad);
	assert(y1 != 0);
	z1 = N1 * (1 - e) * sin(P1_latitude_rad);
	assert(z1 != 0);
	lambda_1 = atan(y1 / x1);

	epsilon = e / (1 - e);

	_b = a * (1 - f);

	p1 = pow((x1 * x1) + (y1 * y1), 0.5);
	q1 = atan((z1 * a) / (p1 * _b));
	h_1 = (p1 / cos(P1_latitude_rad)) - N1;
	phi_1 = atan((z1 + epsilon * _b * pow(sin(q1), 3)) / (p1 - e * a * pow(cos(q1), 3)));

	N2 = a / sqrt(1 - e * pow(sin(P2_latitude_rad), 2));
	x2 = N2 * cos(P2_latitude_rad) * cos(P2_longitude_rad);
	assert(x2 != 0);
	y2 = N2 * cos(P2_latitude_rad) * sin(P2_longitude_rad);
	assert(y2 != 0);
	z2 = N2 * (1 - e) * sin(P2_latitude_rad);
	assert(z2 != 0);
	lambda_2 = atan(y2 / x2);
	p2 = pow((x2 * x2) + (y2 * y2), 0.5);
	q2 = atan((z2 * a) / (p2 * _b));
	h_2 = (p2 / cos(P2_latitude_rad)) - N2;
	phi_2 = atan((z2 + epsilon * _b * pow(sin(q2), 3)) / (p2 - e * a * pow(cos(q2), 3)));

	S_geocent = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) + pow(z2 - z1, 2));
}
void Sphere::showAnswers() const
{
	cout.precision(10);
	cout << fixed << "Эксцентриситет (e) (2.26) = " << sqrt(e) << "\n";
	cout << fixed << "N, м (2.24) = " << N << "\n";
	cout << fixed << "M, м (2.25) = " << M << "\n";
	cout << fixed << "n (2.28) = " << sqrt(eta_2) << "\n";
	cout << fixed << "V (2.29) = " << sqrt(V_2) << "\n";
	cout << fixed << "f4 (2.16) = " << f4 << "\n";
	cout << fixed << "f5 (2.17) = " << f5 << "\n";
	cout << fixed << "f6 (2.18) = " << f6 << "\n";
	cout << fixed << "f7 (2.19) = " << f7 << "\n";
	cout << fixed << "f8 (2.20) = " << f8 << "\n";
	cout << fixed << "s*sin(a) (2.30) = " << s_sina << "\n";
	cout << fixed << "s*cos(a) (2.31) = " << s_cosa << "\n";
	cout << fixed << "delta a, ° (2.32) = " << Grad(delta_a) << "\n";
	cout << fixed << "S, м (2.33) = " << S << "\n";
	cout << fixed << "a1, ° (2.35) = " << (Grad(a1) > 360 ? Grad(a1) - 360 : Grad(a1)) << "\n";
	cout << fixed << "a2, ° (2.36) = " << (Grad(a2) > 360 ? Grad(a2) - 360 : Grad(a2)) << "\n";
	cout << fixed << "ф2, ° (2.37) = " << Grad(phi2_1) << "\n";
	cout << fixed << "lambda2, ° (2.38) = " << Grad(lambda2_1) << "\n";

	for (size_t i = 0; i < iterations.size(); ++i)
	{
		cout << fixed << "-----------Итерация " << i + 1 << "------------------\n";
		cout << fixed << "delta a, ° (2.39) = " << Grad(delta_a(i)) << "\n";
		cout << fixed << "a, ° (2.49) = " << (Grad(a(i)) > 360 ? Grad(a(i)) - 360 : Grad(a(i))) << "\n";
		cout << fixed << "a2, ° (2.41) = " << (Grad(a2(i)) > 360 ? Grad(a2(i)) - 360 : Grad(a2(i))) << "\n";
		cout << fixed << "lambda, ° (2.42) = " << Grad(iterations[i].at(19).second) << "\n";
		cout << fixed << "phi, ° (2.43) = " << Grad(iterations[i].at(20).second) << "\n";
	}
	cout << "---------------------------------------\n";
	cout << fixed << "X1, м(2.5) = " << x1 << "\n";
	cout << fixed << "Y1, м(2.5) = " << y1 << "\n";
	cout << fixed << "Z1, м(2.5) = " << z1 << "\n";
	cout << fixed << "X2, м(2.5) = " << x2 << "\n";
	cout << fixed << "Y2, м(2.5) = " << y2 << "\n";
	cout << fixed << "Z2, м(2.5) = " << z2 << "\n";
	cout << fixed << "S_geocent, м (1.3) = " << S_geocent << "\n";
	cout << fixed << "eps, (2.8)= " << epsilon << "\n";
	cout << fixed << "b, м (2.9) = " << _b << "\n";
	cout << fixed << "p1, (2.10) = " << p1 << "\n";
	cout << fixed << "p2, (2.10) = " << p2 << "\n";
	cout << fixed << "q1, (2.11) = " << q1 << "\n";
	cout << fixed << "q2, (2.11) = " << q2 << "\n";
	cout << fixed << "lambda1, ° (2.6) = " << lambda_1 << "\n";
	cout << fixed << "lambda2, ° (2.6) = " << lambda_2 << "\n";
	cout << fixed << "ф1, ° (2.7) = " << phi_1 << "\n";
	cout << fixed << "ф2, ° (2.7) = " << phi_2 << "\n";

	abs(S_geocent - S);
}