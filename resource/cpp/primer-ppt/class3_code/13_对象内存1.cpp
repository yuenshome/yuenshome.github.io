#include <iostream>
using namespace std;
class A1 {};
class A2 { void fun() {} };
class A3 { int num;	void fun() {} };
class A4 { int num;	char name[6]; };
int main() {
	cout << sizeof(A1) << endl; //1
	cout << sizeof(A2) << endl; //1
	cout << sizeof(A3) << endl; //4
	cout << sizeof(A4) << endl; //12
	return 0;
}