#include <iostream>
using namespace std;
class Base {
	friend ostream& operator<<(ostream &out,const Base &base);
	int b = 1;
};
class D1:public Base  {
	friend ostream& operator<<(ostream &out, const D1 &d1);
	int d = 2;
};
ostream& operator<<(ostream &out, const Base &base) {
	out << "b= " << base.b;
	//out << base.d; //error,Base的友元只能访问Base的成员
	return out;
}
ostream& operator<<(ostream &out, const D1 &d1) {
	//out << d1.b; //error,D1的友元只能访问D1的成员
	out << static_cast<const Base&>(d1) << endl;
	out << "d= " << d1.d ;
	return out;
}
int main() {
	Base b1;
	cout << b1 << endl;
	cout << "--------" << endl;
	D1 d1;
	cout << d1 << endl;
	return 0;
}
