#include <iostream>
#include <cstring>
#include <string>
#include <utility>
#include <vector>
using namespace std;

class myString {
public:
	myString(const char *str = nullptr); //构造
	myString(const myString &other);     //拷贝构造
	//myString(myString &&other) noexcept; //移动构造
	myString &operator=(const myString &other);//赋值运算符重载
	//myString &operator=(myString &&other) noexcept; //移动赋值运算符重载
	~myString() {
		if (ps)
			cout << ps << " --Destructor" << endl;
		else
			cout << "ps is NULL" << " --Destructor" << endl;
		delete[] ps;
	}
	const char * c_str()const { return ps; }
private:
	char *ps;
};

myString::myString(const char *str) {
	if (str == nullptr) {
		ps = new char[1]{ 0 };
		cout << ps << " --Default constructor" << endl;
	}
	else {
		int length = strlen(str);
		ps = new char[length + 1];
		strcpy(ps, str);
		cout << ps << " --Str constructor" << endl;
	}
}

myString::myString(const myString &other) {//拷贝构造
	int length = strlen(other.ps);
	ps = new char[length + 1];
	strcpy(ps, other.ps);
	cout << ps << " --Copy constructor" << endl;
}
//myString::myString(myString &&other) noexcept:ps(other.ps)  { //移动构造
//	other.ps = nullptr; //要保证：移后源对象能正常析构
//	cout << ps << " --Move constructor" << endl;
//}
myString &myString::operator=(const myString &other) { //赋值运算符重载
	if (this != &other) {
		delete[] ps;
		int length = strlen(other.ps);
		ps = new char[length + 1];
		strcpy(ps, other.ps);
	}
	cout << ps << " --Copy assignment" << endl;
	return *this;
}

//myString &myString::operator=(myString &&other) noexcept {//移动赋值运算符重载
//	if (this != &other) {
//		delete[] ps;
//		ps = other.ps;
//		other.ps = nullptr;
//	}
//	cout << ps << "--Move assignment" << endl;
//	return *this;
//}

myString operator+(const myString &a, const myString &b) {
	int length = strlen(a.c_str()) + strlen(b.c_str());
	char * p = new char[length + 1];
	strcpy(p, a.c_str());
	strcat(p, b.c_str());
	myString tmp(p);
	delete[] p;
	return tmp;
}


//myString 实现了普通构造，拷贝构造，赋值运算符重载，析构
int main() {
	vector<myString> vs1;
	vs1.reserve(3);
	cout << "size=" << vs1.size() << " capacity=" << vs1.capacity() << endl;
	for (int i = 0; i < 3; i++) {
		cout << "--------------------------" << endl;
		vs1.push_back(myString("abc"));
		cout << "size=" << vs1.size() << " capacity=" << vs1.capacity() << endl;
		cout << &vs1[0] << endl; //观察连续内存的起始地址
	}
	cout << "==========================" << endl;
	vs1.push_back(myString("12345"));
	cout << "size=" << vs1.size() << " capacity=" << vs1.capacity() << endl;
	cout << &vs1[0] << endl; //观察连续内存的起始地址
	cout << &(*vs1.begin()) << endl;
	cout << "==========================" << endl;
	return 0;
}