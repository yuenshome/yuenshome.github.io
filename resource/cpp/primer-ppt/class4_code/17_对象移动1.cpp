#include <iostream>
#include <cstring>
using namespace std;

class myString {
public:
	myString(const char *str = nullptr); //构造
	myString(const myString &other);     //拷贝构造
	myString &operator=(const myString &other);//赋值运算符重载
	~myString() {
		cout << ps << " --Destructor" << endl; 
		delete[] ps;
	}
	const char * c_str()const { return ps; }
private:
	char *ps;
};

myString::myString(const char *str)
{
	if (str == nullptr) {
		ps = new char[1]{0};
		cout << ps << " --Default constructor" << endl;
	}
	else {
		int length = strlen(str);
		ps = new char[length + 1];
		strcpy(ps, str);
		cout << ps << " --Str constructor" << endl;
	}
}

myString::myString(const myString &other)
{
	int length = strlen(other.ps);
	ps = new char[length + 1];
	strcpy(ps, other.ps);
	cout << ps << " --Copy constructor" << endl;
}

myString &myString::operator=(const myString &other)
{
	if (this != &other) {
		delete[] ps;
		int length = strlen(other.ps);
		ps = new char[length + 1];
		strcpy(ps, other.ps);
	}
	cout << ps << " --Copy assignment" << endl;
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
	myString s4 = s1 + s2; 
	// s1 + s2 是临时量,拷贝给s4后,马上析构了
	cout << "2====================" << endl;
	myString s5 = fun();
	cout << "3====================" << endl;

	return 0;
}