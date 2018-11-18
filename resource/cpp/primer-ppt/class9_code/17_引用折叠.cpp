#include <iostream>
using namespace std;

//模板：允许将一个 右值引用 绑定到 一个左值
//一: 将一个左值传递给函数的右值引用参数时,并且此右值引用指向模板类型参数（如T&&）时，推断模板类型参数是实参的左值引用类型。
//二: 引用折叠,类型T& &、T& &&、T&& & 折叠成类型 T&, 类型 T&& && 折叠成类型 T&&

//实参必须是左值
template <typename T> void f1(T&) {}
//实参可以是右值
template <typename T> void f2(const T&) {}
//模板：实参可以是左值
template <typename T> void f3(T&&) {}

template <typename T> void f4(T&& val) {
	T t = val;
	//1.假如传入一个int左值: T 就是 int&, 那么上面相当于 int& t = val;
	//2.假如传入一个int右值: T 就是 int, 那么上面相当于 int t = val;

	t = 100; //情况1: val将会置为100, 情况2: val不会改变
	cout << "f4(): val = " << val << endl;
}

int main() {
	int i = 10; 
	const int ci = 20;
	f1(i);  // T: int
	f1(ci); // T: const int
	//f1(2);// 错,不能是右值
	f2(i);  // T: int
	f2(ci); // T: int
	f2(2);  // T: int
	f3(i);  // T: int &
	f3(ci); // T: const int &
	f3(2);  // T: int

	f4(i);
	cout << "左值i= "<<i << endl;
	i = 10;
	f4(std::move(i));
	cout << "右值i="<<i << endl;
	return 0;
}