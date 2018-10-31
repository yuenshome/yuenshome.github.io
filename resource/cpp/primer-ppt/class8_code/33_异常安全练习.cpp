#include <iostream>
#include <memory>
using namespace std;

class A {
public:
	A(int i):p_data(new int(i)){
		if (i % 2) throw "A_Error";
	} //奇数初始化会抛异常
	int get_data()const { return *p_data; }
	void swap(A& rhs) {
		std::swap(p_data, rhs.p_data);
	}
private:
	unique_ptr<int> p_data;
};

class Test {
public:
	Test(int i, int j) :a1(i), a2(j) {}
	void print_a1_a2()const {
		cout << "a1=" << a1.get_data()
			<< " a2=" << a2.get_data() << endl;
	}
	void set_a1_a2(int i, int j) {
		A tmp_a1(i);
		A tmp_a2(j);
		tmp_a1.swap(a1);
		tmp_a2.swap(a2);
	}
private:
	A a1;
	A a2;
};

int main() {
	Test t(2, 4);
	t.print_a1_a2();
	t.set_a1_a2(100, 200);
	t.print_a1_a2();
	cout << "----------\n";
	//当前是 100,200
	try {
		t.set_a1_a2(100, 201);
	}
	catch (const char * e) {
		cout << e << endl;
	}
	t.print_a1_a2();
	//仍然是100,200
	//异常安全，操作失败没有改变原始值
	return 0;
}