#include <iostream>
#include <iomanip>
using namespace std;
int main() {
	// int ,float, double的转换
	int i = 1234567890;
	float f = 1.234f;
	double fd = 3.1415926;
	f = i; //(丢失精度)
	cout << "f= " << setw(10) << setprecision(2)
		<< setiosflags(ios::fixed) << f << endl;
	i = fd; //i = 3 (小数点后面丢失)
	cout << "i= " << i << endl;
	f = fd; //double转float,(精度丢失)
	cout << "f= " << setw(10) << setprecision(7)
		<< setiosflags(ios::fixed) << f << endl;

	return 0;
}

