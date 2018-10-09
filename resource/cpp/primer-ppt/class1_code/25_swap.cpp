#include <iostream>
using namespace std;
void swap1(int *pa, int *pb) {
	int tmp = *pa;
	*pa = *pb;
	*pb = tmp;
}
void swap2(int &a, int &b) {
	int tmp = a;
	a = b;
	b = tmp;
}
void swap3(int a, int b) {
	int tmp = a;
	a = b;
	b = tmp;
}

int main() {
	int a = 10, b = 20;
	swap1(&a, &b);
	cout << a << b << endl; //20 10
	swap2(a, b);
	cout << a << b << endl;//10 20
	swap3(a, b);
	cout << a << b << endl;//10 20 ½»»»Ê§°Ü
	return 0;
}

