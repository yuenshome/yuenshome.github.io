#include <iostream>
#include <cstring>
#include <string>
#include <utility>
using namespace std;

class myString {
public:
	myString(const char *str = nullptr); //构造
	myString(const myString &other);     //拷贝构造
	myString(myString &&other);			 //移动构造
	myString &operator=(const myString &other);//赋值运算符重载
	myString &operator=(myString &&other); //移动赋值运算符重载
	~myString() {
		if(ps)
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
myString::myString(myString &&other):ps(other.ps) { //移动构造
	other.ps = nullptr;
	cout << ps << " --Move constructor" << endl;
}
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

myString &myString::operator=(myString &&other) {//移动赋值运算符重载
	if (this != &other) {
		delete[] ps;
		ps = other.ps;
		other.ps = nullptr;
	}
	cout << ps << "--Move assignment" << endl;
	return *this;
}

myString operator+(const myString &a, const myString &b) {
	int length = strlen(a.c_str()) + strlen(b.c_str());
	char * p = new char[length + 1];
	strcpy(p, a.c_str());
	strcat(p, b.c_str());
	myString tmp(p);
	delete[] p;
	return tmp;
}

myString fun() {
	myString tmp("allok");
	return tmp;
}
int main() {
	myString s1("abc");
	myString s2("123");
	myString s3 = s1; //拷贝构造
	cout << "1====================" << endl;
	myString s4(myString("ok"));
	cout << "2====================" << endl;
	myString s5 = s1 + s2; 
	// s1 + s2 是临时量(右值),调用移动构造给s4,马上析构
	cout << "3====================" << endl;
	myString s6 = fun();
	// fun()返回值 是临时量(右值),调用移动构造给s6,马上析构
	cout << "4====================" << endl;
	myString s7 = std::move(s6); //std::move函数，#include <utility>
	cout << (void*)s6.c_str() << endl; //s6中的资源ps=NULL了。
	cout << "5====================" << endl;
	return 0;
}