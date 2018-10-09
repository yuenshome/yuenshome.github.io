#include <iostream>
#include <string.h>
using namespace std;

class myString {
public:
	myString(const char *str = NULL);
	~myString();
private:
	char *ps; int size;
};
myString::myString(const char *str) {
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
myString::~myString() {
	delete[] ps;
	cout << "~myString()" << endl;
}

class Person {
public:
	Person() { age = 20; p_fm = NULL; }
	void set_fm(){
		p_fm = new myString[2];
	}
	~Person() { 
		cout << "~Person()" << endl;
		delete[] p_fm; 
		cout << "=========" << endl;
	}
private:
	int age;        //年龄
	myString name;  //自己的名字
	myString *p_fm; //父母的名字
};

int main() {
	Person p1;
	p1.set_fm();
	return 0;
}
