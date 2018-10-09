#include <iostream>
using namespace std;
struct Comp {
	int real;
	int image;
};
Comp operator+(const Comp &a, const Comp &b) {
	Comp res = { a.real + b.real,a.image + b.image };
	return res;
}
int main() {
	Comp a = { 1,2 }, b = { 10,20 };
	Comp c = a + b;			  //相当与 operator+(a,b)
	Comp d = operator+(a, b); //相当于 a + b
	cout << c.real << "," << c.image << "i" << endl;
	cout << d.real << "," << d.image << "i" << endl;
	return 0;
}