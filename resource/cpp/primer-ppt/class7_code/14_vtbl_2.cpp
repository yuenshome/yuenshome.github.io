#include <iostream>
using namespace std;
class Base {
public:
	Base() { 
		cout << "Base构造\n"; 
		memset(this, 0, sizeof(*this)); 
		//构造时,强制将对象的所有数据置为0
	}
	void func() { cout << "Base::func()\n"; }
	virtual void show()const { cout << "Base::show()\n"; }
private:
	int b;
};
class D1 :public Base {
public:
	D1() { cout << "D1构造\n"; 	}
	virtual void show()const override {
		cout << "D1::show()\n"; 
	}
private:
	int d;
};
int main() {
	Base b1;
	b1.func(); //非虚函数调用,ok
	b1.show(); //虚函数调用,ok
	//上面虚函数调用ok,是因为通过对象调用虚函数,也是编译时确定的,没有通过虚函数表
	Base *pb1 = &b1;
	pb1->func(); //非虚函数调用,ok
	//pb1->show(); //程序崩溃,指向虚函数表的指针被破坏了.
	//通过指针或引用调用虚函数,会使用虚函数表,运行时确定。
	cout << "------------\n";
	D1 d;
	Base &rd = d;
	rd.func(); //ok,非虚函数调用
	rd.show(); //ok,D1构造时,重建了指向虚函数表的指针(因为Base和D1的虚函数表不同)
	return 0;
}