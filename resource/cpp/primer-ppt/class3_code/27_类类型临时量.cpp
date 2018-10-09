#include <iostream>
using namespace std;

// 临时量：内置类型是const的，类类型不一定
class A {
public:
	A(const char* s = NULL){}
	A& operator+(const A& other) {
		cout << "+++" << endl;
		return *this;
	}
	const A& operator-(const A& other) {
		cout << "---" << endl;
		return *this;
	}
};
void fun(const int &i) { cout << "const int" << endl; }
void fun(int &i) { cout << "int" << endl; }
void fun(const A &a) { cout << "const A" << endl; }
void fun(A &a) { cout << "A" << endl; }
int main() {
	fun(1+1); //调用const版本
	cout << "-----" << endl;
	int i = 10;
	fun(i); //调用非const版本
	cout << "-----" << endl;
	A a;
	fun(a + "aa"); //调用非const
	fun(a - "aa"); //调用const
	return 0;
}
