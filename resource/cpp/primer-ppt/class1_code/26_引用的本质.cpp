#include <iostream>
using namespace std;

struct Stu {
	int age;
	char sex;
	char name[20];
};
struct A {
	int &data;
};
struct B {
	char &data;
};
struct C {
	Stu &data;
};

int main() {
	cout << sizeof(A) << endl; //4
	cout << sizeof(B) << endl; //4
	cout << sizeof(C) << endl; //4
	return 0;
}