#include <iostream>
#include "invGeod.h"
using namespace std;
int main()
{
	cout.precision(9);
	setlocale(LC_CTYPE, "rus");
	cout << "������� ��� �������: ";
	variant var;
	cin >> var;
	invGeod timLox(var);
	timLox.show();
	return 0;
}