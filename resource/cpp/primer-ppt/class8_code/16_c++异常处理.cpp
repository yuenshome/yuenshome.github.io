#include <iostream>
#include <cmath>
using namespace std;
double triangle_area(double a, double b, double c) {
	if (a + b > c && a + c > b && b + c > a) {
		double p = (a + b + c) / 2;
		double area = sqrt(p*(p - a)*(p - b)*(p - c));
		return area;
	}
	throw - 1;
	cout << "triangle_area(): ´íÎó\n"; //ÓÀÔ¶Ö´ÐÐ²»µ½
}

int main() {
	double s1 = 0.0;
	try {
		s1 = triangle_area(2, 2, 4);
	}
	catch (int e) {
		cout << "Main: " << e << endl;
	}
	cout << "Main: end\n";
	return 0;
}