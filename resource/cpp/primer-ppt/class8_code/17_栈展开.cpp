#include <iostream>
using namespace std;
class Err {};
void f4() {
	cout << "f4():1\n";
	throw 3.14;
	throw "abc";
	throw false;
	throw Err();
	throw (short)1;
	cout << "f4():2\n"; //²»Ö´ÐÐ
}
void f3() {
	cout << "f3():1\n";
	try {
		try {
			f4();
		}
		catch (int e) {
			cout << "f3(): err_int\n";
		}
		catch (const char* e) {
			cout << "f3(): err_char*\n";
		}
	}
	catch (short e) {
		cout << "f3(): err_short\n";
	}
	cout << "f3():2\n";
}
void f2() {
	cout << "f2():1\n";
	f3();
	cout << "f2():2\n";
}
void f1() {
	cout << "f1():1\n";
	try {
		f2();
	}
	catch (double e) {
		cout << "f1(): err_double\n";
	}
	catch (Err e) {
		cout << "f1(): err_Err\n";
	}
	cout << "f1():2\n";
}


int main() {
	f1();
	try {
		f4();
	}
	catch (...) {
		cout << "main: err_default\n";
	}
	return 0;
}