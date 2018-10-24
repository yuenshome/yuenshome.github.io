#include <iostream>
#include <typeinfo>
using namespace std;
class Base {
public:
	Base(int b1 = 0) :b(b1) {}
	virtual ~Base() = default;
	virtual bool equal(const Base& rhs)const {
		return b == rhs.b;
	}
private:
	int b;
};
class Derived :public Base {
public:
	Derived(int b1=0, int d1=0) :Base(b1), d(d1) {}
	//注意：这里的参数只能写 Base, 否则就不是 override
	virtual bool equal(const Base& rhs)const override {
		//注意下面 const的用法
		const Derived *pd = dynamic_cast<const Derived*>(&rhs);
		if (pd)
			return Base::equal(rhs) && d == pd->d;
		return false;
	}
private:
	int d;
};
bool operator==(const Base& lhs, const Base& rhs) {
	return typeid(lhs)==typeid(rhs) && lhs.equal(rhs);
}
bool operator!=(const Base& lhs, const Base& rhs) {
	return !(lhs == rhs);
}
int main() {
	Base b1(1), b2(1);
	if (b1 == b2) cout << "b1==b2" << endl; //b1==b2
	Base b3(1), b4(2);
	if (b3 != b4) cout << "b3!=b4" << endl; //b3!=b4
	Derived d1(1, 2), d2(1, 2);
	if (b1 != d1) cout << "b1!=d1" << endl; //b1!=d1
	if (d1 == d2) cout << "d1==d2" << endl; //d1==d2
	Derived d3(1, 2), d4(1, 4);
	if (d3 != d4) cout << "d3!=d4" << endl; //d3!=d4
	return 0;
}