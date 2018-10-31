#include <iostream>
using namespace std;
class A {
public:
	A() { cout << "A构造\n"; }
	~A() noexcept(false) {
		throw 1; cout << "A析构\n"; 
	} //析构函数中有异常逃离
};
void f() {
	A a1;
	A a2;
	throw 2;
}
int main() {
	try { f(); }
	catch (...) { }
	return 0;
}