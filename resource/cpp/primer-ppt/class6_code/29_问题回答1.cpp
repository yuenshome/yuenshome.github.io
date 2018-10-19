#include <iostream>
using namespace std;
class Base {
public:
	virtual void func() = 0;
	virtual void func(double) {
		cout << "Base(double)\n";
	}
	void func(int) {
		cout << "Base(int)\n";
	}
};
class D1 : public Base {
public:
	virtual void func() override {
		cout << "D1()\n";
	}
	void func(const char*) {
		cout << "D1(const char*)\n";
	}
};
class D2 :public D1 {
public:
	using Base::func;
	void func(bool) {
		cout << "D2(bool)\n";
	}
};

int main() {
	//Base b1;				//问题1：错误,有纯虚函数,抽象基类无法创建对象
	D1 d1;
	d1.func();				//问题2：正确, D1 override了func(), 输出：D1()
	//d1.func(1.2);			//问题3：错误, D1重新定义了func,shadow了基类的所有func函数
	//d1.func(1);			//问题4：错误, 同问题3
	d1.func("abc");			//问题5：正确 D1(const char*)
	D2 d2;
	d2.func();				//问题6：ok,using Base::func,将Base的所有func函数的重载实例添加到D2的作用域，D1() [注意]
	d2.func(1.2);			//问题7：同问题6，Base(double)
	d2.func(1);				//问题8：同问题6，Base(int)
	d2.func("123");			//问题9：ok(注意),D1中的func(const char*)被shadow,隐式转换执行func(bool) D2(bool)
	d2.func(true);			//问题10：ok D2(bool)
	cout << "--------------------------\n";
	Base *p1 = new D1;
	p1->func();				//问题11：虚函数调用,真实类型的调用 D1()
	p1->func(1.2);			//问题12：虚函数调用,但是D1没有override Base(double)
	p1->func(1);			//问题13：非虚函数调用,静态类型的调用 Base(int)
	//p1->func("123");		//问题14: 错误,静态类型中找不到 func(const char*)
	p1->func(true);			//问题15：隐式转换 Base(int)
	D1 *p2 = new D2;
	p2->func();				//问题16：虚函数调用,真实类型的调用 D1() (D2没有override func(),所以是D1中的)
	//p2->func(1.2);		//问题17：错误,静态类型D1中没有 func(double),也没有可以隐式转换匹配的
	//p2->func(1);			//问题18：错误,同上
	p2->func("123");		//问题19: D1(const char*)
	//p2->func(true);		//问题20：错误,静态类型D1中没有 func(bool),也没有可以隐式转换匹配的
	return 0;
}
