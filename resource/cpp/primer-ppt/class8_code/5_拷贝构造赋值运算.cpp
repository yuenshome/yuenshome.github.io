#include <iostream>
using namespace std;
struct A { int ai; A(int i) :ai(i){} };
struct B { int bi; B(int i) :bi(i){} };
class Base1 {
public:
	Base1(int ii):i(ii){}
	int i;
};
class Base2 {
public:
	Base2(int jj):j(jj) {}
	int j;
};
class D1 :public Base1 {
public:
	D1(int i):Base1(i),a(i){}
	A a;
};
class Ok :public D1, public Base2 {
public:
	Ok(int i):D1(i),Base2(i),b(i) { cout << "Ok()\n"; }
	Ok(const Ok& other)
		:D1(other),Base2(other),b(other.b){}
	Ok& operator=(const Ok& other) {
		if (this == &other) return *this;
		D1::operator=(other);    //调用基类赋值
		Base2::operator=(other); //调用基类赋值
		b = other.b;
		return *this;
	}
	B b;
};
int main() {
	Ok ok1(10);
	cout << ok1.a.ai << " " << ok1.b.bi << endl;
	Ok ok2(1);
	ok2 = ok1;
	cout << ok2.a.ai << " " << ok2.b.bi << endl;
	return 0;
}

