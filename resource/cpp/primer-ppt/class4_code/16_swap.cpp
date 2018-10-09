#include <iostream>
#include <cstring>
using namespace std;
class myString {
public:
	friend void swap(myString &a, myString &b);
	myString(const char * pstr = NULL) {
		if (!pstr) {
			ps = new char[1];
			ps[0] = '\0';
		}
		else {
			ps = new char[strlen(pstr) + 1];
			strcpy(ps, pstr);
		}
		cout << "Constructor: const char*" << endl;
	}
	myString(const myString &other) {
		ps = new char[strlen(other.ps) + 1];
		strcpy(ps, other.ps);
		cout << "Constructor: copy" << endl;
	}
	myString &operator=(const myString &other) {
		if (this != &other) {
			delete[] ps;
			ps = new char[strlen(other.ps) + 1];
			strcpy(ps, other.ps);
		}
		cout << "operator = " << endl;
		return *this;
	}
	~myString() {
		delete[] ps;
		cout << "Delete[]" << endl;
	}
private:
	char *ps;
};

void std_swap(myString &a, myString &b) {
	myString tmp = a;
	a = b;
	b = tmp;
}
void swap(myString &a, myString &b) {
	std::swap(a.ps, b.ps);
}
int main() {
	myString s1 = "abc";
	myString s2 = "124";
	cout << "=============" << endl;
	std::swap(s1, s2); //std::swap函数
	cout << "=============" << endl;
	std_swap(s1, s2);  //模拟std::swap
	cout << "=============" << endl;
	//std::swap执行了深拷贝（拷贝构造和赋值运算符重载）
	swap(s1, s2); //自己定义的swap函数，没有深拷贝
	cout << "=============" << endl;
	return 0;
}