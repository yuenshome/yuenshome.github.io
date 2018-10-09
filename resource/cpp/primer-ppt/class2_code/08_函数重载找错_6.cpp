#include <iostream>
using namespace std;
void f(const int* p) { cout << 1; }
void f(int *p) { cout << 2; }
int main() {
	int i = 1;
	const int j = 2;
	int *pi = &i;
	const int *pj = &j;
	f(pi);
	f(pj);
	//f(nullptr);
	//f(0); //´íÎó
	return 0;
}