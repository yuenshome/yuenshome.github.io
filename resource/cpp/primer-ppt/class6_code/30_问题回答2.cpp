#include <iostream>
#include <memory>
using namespace std;
class Base {
public:
	Base(int i):p_base(new int(i)){}
	~Base() { delete p_base; }
	virtual void show()const {
		cout << "Base: " << *p_base << endl;;
	}
private:
	int *p_base;
};
class D1 : public Base {
public:
	D1(int i,int j):Base(i),p_d1(new int(j)){}
	~D1() { delete p_d1; }
	virtual void show()const override {
		cout << "D1: " << *p_d1 << endl;;
	}
private:
	int *p_d1;
};

void print(const Base &b) {
	b.Base::show();
	b.show();
}

int main() {
	auto pd1 = make_shared<D1>(1, 2); //pd1的类型是什么
	print(*pd1); //运行结果？
	//智能指针，一样有多态
	unique_ptr<Base> pd2 = make_unique<D1>(3, 4);
	print(*pd2); //运行结果?

	return 0; //这个代码整体上有什么问题？
}

