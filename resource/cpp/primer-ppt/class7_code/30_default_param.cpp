#include <iostream>
using namespace std;

class Base {
public:
	virtual ~Base() = default;
	void f1(int i=0) { 
		cout << "Base f1():" << i << endl;
	}
	virtual void vf2(int i = 0) { 
		cout << "Base vf2():" << i << endl; 
	}
};
class D1 :public Base {
public:
	void f1(int i=1) {
		cout << "D1 f1():"<<i << endl; 
	}
	virtual void vf2(int i = 1)override{ 
		cout << "D1 vf2():" << i << endl; 
	}
};

int main() {
	Base b;
	b.f1(); //0
	b.vf2();//0
	D1 d;
	d.f1(); //1
	d.vf2();//1
	Base *pb = &d;
	pb->f1(); //0
	pb->vf2();//0 不符合预期: 默认参数是 Base::vf2中的
	D1 *pd = &d;
	pd->f1(); //1
	pd->vf2();//1 编译时确定哪个函数,哪个默认参数
	return 0;
}