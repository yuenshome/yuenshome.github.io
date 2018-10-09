#include <iostream>
using namespace std;

int main() {
	//1.auto 变量必须在定义时初始化，类似于const
	auto i1 = 0; auto i2 = i1;
	//auto i3; //错误，必须初始化
	//2.如果初始化表达式是引用，则去除引用语义
	int a1 = 10;
	int &a2 = a1; // a2是引用
	auto a3 = a2; // a3是int类型，而不是引用
	auto &a4 = a1; // a4是 引用
	//3.去除顶层const
	const int b1 = 100;
	auto b2 = b1; // b2 是 int
	const auto b3 = b1; // b3是 const int
	//4.带上底层const
	auto &b4 = b1; // b4 是 const int 的引用
	//5.初始化表达式为数组时，推导类型为指针
	int arr[3] = { 1,2,3 };
	auto parr = arr; //parr 是 int * 类型
	cout << typeid(parr).name() << endl;
	//6.表达式为数组且auto带上&，推导类型为数组
	auto &rarr = arr; //rarr 是 int [3]
	cout << typeid(rarr).name() << endl;
	//7.函数参数类型不能是 auto
	//func(auto arg); //错误
	//8.auto并不是一个真正的类型,编译时确定
	//sizeof(auto); 错误
	return 0;
}