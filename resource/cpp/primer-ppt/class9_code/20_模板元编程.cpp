#include <iostream>
using namespace std;

template<unsigned N>
struct Factorial { //½×³Ë
	enum { value = N * Factorial<N - 1>::value };
};

template<>
struct Factorial<0> {
	enum { value = 1 };
};

int main() {
	cout << Factorial<4>::value << endl;
	cout << Factorial<16>::value << endl;
	cout << "------\n";
	return 0;
}