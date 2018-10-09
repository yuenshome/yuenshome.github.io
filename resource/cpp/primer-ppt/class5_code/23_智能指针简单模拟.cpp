#include <iostream>
using namespace std;

class T {
public:
	T(int i = 0) :data(i) { cout << "T构造\n"; }
	~T() { cout << "T析构\n"; }
private:
	int data;
};

class SmartPointer {
public:
	SmartPointer(T* p = nullptr) :ptr(p) { }
	~SmartPointer() { delete ptr; }
private:
	T * ptr;
};

int main() {
	T * pt1 = new T(1);
	SmartPointer spt1(pt1);
	T * pt2 = new T(2);
	SmartPointer spt2 = pt2;
	//函数结束时，pt1,pt2管理的俩个 T 对象都会析构
	cout << "------\n";
	return 0;
}