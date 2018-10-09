#include <iostream>
using namespace std;
class A {
public:
	A() { cout << "A()" << endl; }
	~A() { cout << "~A()" << endl; }
};
void fun() { A a2; }
int main() {
	{
		A a1;
	}
	//到这里，a1已经析构了。
	fun();
	//到这里，fun中的a2已经析构了。
	cout << "----------" << endl;
	int i = 0;
	//注意a3的作用域
	for (A a3; i < 3; i++) {
		cout << i << endl;
	}
	cout << "----------" << endl;
	//注意a4的作用域
	for (int j = 0; j < 3; j++) {
		A a4; //每次循环都会构造
		cout << j << endl;
	}//while循环也是如此
	cout << "----------" << endl;
	A *pa = new A; //构造
	delete pa; //调用delete析构
	return 0;
}