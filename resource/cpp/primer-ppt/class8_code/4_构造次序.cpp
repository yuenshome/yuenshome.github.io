#include <iostream>
using namespace std;
struct A { A() { cout << "A()\n";} };
struct B { B() { cout << "B()\n"; } };
class Base1 {
public:
	Base1() { cout << "Base1()\n"; }
private:
	int i;
};
class Base2 {
public:
	Base2(int jj):j(jj) { cout << "Base2()\n"; }
private:
	int j;
};
class D1 :public Base1 {
public:
	D1() { cout << "D1()\n"; }
	//实际是：D1():Base1(),a()
private:
	A a;
};
class Ok :public D1, public Base2 {
public:
	Ok():Base2(2) { cout << "Ok()\n"; }
	//实际是：Ok():D1(),Base2(2),b()
private:
	B b;
};
int main() {
	Ok ok;
	return 0;
}