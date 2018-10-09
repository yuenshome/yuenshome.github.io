#include <iostream>
#include <cassert>
using namespace std;
class T {
public:
	T(int i = 0) :data(i) { cout << "T构造\n"; }
	~T() { cout << "T析构\n"; }
	void show()const { cout << data << endl; }
private:
	int data;
};
class SmartPointer {
public:
	SmartPointer(T* p = nullptr)
		:ptr(p), count(new int(ptr ? 1 : 0)) {	}
	~SmartPointer() {
		if (--(*count) <= 0) {
			delete ptr;	delete count;
		}
	}
	SmartPointer(const SmartPointer& other)//拷贝构造
		:ptr(other.ptr), count(&(++*other.count)) {	}
	SmartPointer& operator=(const SmartPointer& other) {
		if (this == &other) return *this;  //赋值操作
		++*other.count;
		if (--*this->count <= 0) {
			delete ptr;
			delete count;
		}
		ptr = other.ptr;
		count = other.count;
		return *this;
	}
	T& operator*() { assert(ptr); return *ptr;}
	T* operator->() { assert(ptr); return ptr;}
	operator bool()const { return ptr != nullptr; }
private:
	T * ptr;
	int * count;
};

int main() {
	SmartPointer pd1(new T(1));
	SmartPointer pd2 = pd1; //拷贝构造
	SmartPointer pd3;
	pd3 = pd2; //赋值
	//退出时，只析构了1次
	return 0;
}