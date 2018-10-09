#include <iostream>
using namespace std;
int f1() { return 33; }  //ok
//和上面是一样的,调用时 int i = f2(); ok
const int f2() { int i = 10; return i; }
int *f3() {
	int *p = new int[3]{ 1,2,3 };
	return p;
}
const int *f4() {
	int *p = new int[3]{ 1,2,3 };
	return p;
}
int &f5() {
	int *p = new int(100);
	return *p;
}
const int &f6() {
	int *p = new int(100);
	cout <<"f6():"<< p << endl;
	return *p;
}
//int &f7() { return 10; } //错误
int &f7() { int i = 101; return i; }//编译ok,但是不能这样用
const int &f8() { return 102; } //编译ok,但是不能这样用

int main() {
	//int *p = f4(); //错误
	const int *p = f4(); //必须用const接收
	//int &i = f6(); //错误
	const int &i = f6(); //必须用const接收
	const double &pd = f6(); //pd在栈空间了
	cout << &pd << endl; //上面的转换后,地址不同了
	cout << &p << endl;
	cout << "--------" << endl;
	cout << f7() << endl; //输出101 貌似ok 
	int &rf7 = f7();
	cout << rf7 << endl;  //输出101 貌似也ok
	f1(); //随便调用了一个函数
	cout << rf7 << endl;  //输出-858.... 出错！
	//所以不能返回局部变量的引用
	return 0;
}

