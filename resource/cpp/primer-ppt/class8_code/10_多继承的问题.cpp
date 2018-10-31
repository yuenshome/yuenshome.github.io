#include <iostream>
using namespace std;

class Base {
public:
	int x = 0;
};
class D1 :public Base {
public:
	void set_x(int i) { x = i; }
	int i = 1;
};
class D2 :public Base {
public:
	int get_x()const {return x;}
	int j = 2;
};
class Ok :public D1, public D2 {
public:
	int k = 3;
};
int main() {
	Ok ok;
	ok.set_x(100);
	cout << ok.get_x() << endl;
	return 0;
}


