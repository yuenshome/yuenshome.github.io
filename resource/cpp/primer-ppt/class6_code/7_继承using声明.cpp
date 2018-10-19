#include <iostream>
using namespace std;
class Base {
public:
	void f_pub() {}
	int i_pub;
protected:
	void f_pro() {}
	int i_pro;
private:
	void f_pri() {}
	int i_pri;
};

class D1 :private Base {
public:
	using Base::f_pro;
	using Base::i_pro;
private:
	//using Base::f_pri; ´íÎó,Base::f_pri²»¿É¼û
};

int main() {
	D1 d1;
	d1.f_pro();
	d1.i_pro = 10;
	return 0;
}