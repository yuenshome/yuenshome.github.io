#include <iostream>
using namespace std;

int g_i = 10;
int main() {
	static int s_i = 20;
	int i1 = 10;
	auto f1 = [] {
		cout << g_i << s_i << endl; //全局,static变量可以用
		//cout << i1 << endl; 用不了i1
	};
	f1(); //输出10 20
	auto f2 = [i1](int i) {
		cout << i1 << i <<endl; //值传递
	};
	i1 = 100;
	f2(0); //输出 10 0, 此处注意：i1是定义lambda时的i1,值拷贝
	auto f3 = [&i1] {cout << i1 << endl; }; //引用传递
	i1 = 20;
	f3(); //输出20 引用传递
	int i2 = 30;
	[&] {cout << i1 << i2 << endl; }(); //直接全部引用局部变量
	//输出 20 30
	return 0;
}

