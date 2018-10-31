#include <iostream>
using namespace std;

class Base2 {
public:
	Base2() { cout << "Base2\n"; }
};
class Base {
public:
	Base(int i):x(i) { cout << "Base\n"; }
	int x;
};
class D1 :virtual public Base {
public:
	D1(int ii):Base(ii),i(ii) { cout << "D1\n"; }
	int i;
};
class D2 :virtual public Base {
public:
	D2(int jj):Base(jj),j(jj) { cout << "D2\n"; }
	int j;
};
class Ok :public D1, public D2,public Base2 {
public:
	Ok(int kk):D1(1),D2(2),Base(100),Base2() {
		cout << "Ok\n"; 
	}
	int k;
};
int main() {
	Ok ok(1);
	cout << ok.x << endl; //100
	return 0;
}


