#include <iostream>
using namespace std;
class Base {
public:
	~Base() { cout << "Base析构\n"; }
};
class D1 :public Base {
public:
	D1():pi(new int(0)){ }
	~D1() { delete pi; cout << "delete\n"; }
	int * pi;
};

class Base2 {
public:
	virtual ~Base2() { cout << "Base析构\n"; }
};
class D2 :public Base2 {
public:
	D2() :pi(new int(0)) { }
	~D2() { delete pi; cout << "delete\n"; }
	int * pi;
};

int main() {
	Base *pb = new D1;
	delete pb;
	//pb的静态类型是 Base,所以会调用 Base的析构
	//D1的析构没有运行到，内存泄漏
	cout << "------------\n";

	Base2 *pb2 = new D2;
	delete pb2;
	//因为Base2的析构函数是虚函数
	//所以在调用pb2的析构时,会调用它实际类型的析构
	//pb2的实际类型是D2的指针,正确释放内存
	return 0;
}