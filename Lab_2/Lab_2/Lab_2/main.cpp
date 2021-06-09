#include "sphere.h"
#include <iostream>
#include <clocale>
int main()
{
	setlocale(LC_ALL, "rus");
	Sphere s(12);

	s.showAnswers();
	return 0;
}