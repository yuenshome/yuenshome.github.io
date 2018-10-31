#include <iostream>
using namespace std;
struct A { int ai=1;};
struct B { int bi=2;};
class Base1 {
public:
	int i=3;
};
class Base2 {
public:
	int j=4;
};
class D1 :public Base1 {
public:
	A a;
};
class Ok :public D1, public Base2 {
public:
	B b;
};
int main() {
	Ok ok1;
	cout << ok1.b.bi << endl;
	cout << "ok1的地址: " << &ok1 << endl;
	Base1 *pb1 = &ok1;
	cout << "pb1指向的地址: " << pb1 << endl;
	D1 *pd1 = &ok1;
	cout << "pd1指向的地址: " << pd1 << endl;
	Base2 *pb2 = &ok1;
	cout << "pb2指向的地址: " << pb2 << endl;
	cout << "-------------\n";
	Ok * pok1 = static_cast<Ok*>(pb1);
	cout << pok1->b.bi << endl;
	//Ok * pok2 = dynamic_cast<Ok*>(pb1);
	//不行，dynamic_cast必须有虚函数
	Ok * pok3 = static_cast<Ok*>(pb2);
	cout << "pok3指向的地址: " << pok3 << endl;
	cout << pok3->b.bi << endl;
	Base2 base2;
	Ok * pok4 = static_cast<Ok*>(&base2);
	cout << "base2地址:     " <<&base2 << endl;
	cout << "pok4指向的地址:" << pok4 << endl;
	cout << pok4->b.bi << endl; //未定义,访问了未知内存
	return 0;
}

