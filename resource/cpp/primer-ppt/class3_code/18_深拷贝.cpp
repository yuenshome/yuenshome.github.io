#include <iostream>
#include <string.h>
using namespace std;

class myString {
public:
	myString(const char *str = NULL) {
		if (str == NULL) {
			ps = new char[1];
			ps[0] = '\0';
		}
		else {
			int len = strlen(str) + 1;
			ps = new char[len];
			strcpy(ps, str);
		}
	}
	myString(const myString &other);
	myString& operator=(const myString &other);
	~myString() {
		delete[] ps;
	}
private:
	char *ps;
};

myString::myString(const myString &other) {
	ps = new char[strlen(other.ps) + 1];
	strcpy(ps, other.ps);
} //拷贝构造

myString& myString::operator=(const myString &other) {
	if (this == &other) return *this;
	delete[] ps;
	ps = new char[strlen(other.ps) + 1];
	strcpy(ps, other.ps);
	return *this;
}//赋值运算符重载

int main() {
	myString s1("AB");
	myString s2 = s1;
	s2 = s1;

	return 0;
}
