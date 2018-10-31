#include <iostream>
using namespace std;

void f1() noexcept { } //不会抛异常
void f2() noexcept(true){}//不会抛异常
void f3() noexcept(false) {}//可能抛异常
void f4() {} //可能抛异常
void f5() throw() {} //不会抛异常
void f6() throw(int,double){} //只会抛int和double类型的异常
void f7() noexcept { //不会抛异常
	throw 1; //实际上抛了,编译ok,实际调用std::terminate
}
void f8() noexcept {
	try { throw 1; }
	catch (int e) { cout << "异常处理！\n"; }
	//异常处理了，ok,没问题。
}

int main() {
	f8(); //正常运行
	//noexcept运算符：不会抛异常返回true,会抛异常返回false
	cout << noexcept(f1()) << endl; //1
	cout << noexcept(f2()) << endl; //1
	cout << noexcept(f3()) << endl; //0
	cout << noexcept(f4()) << endl; //0
	cout << noexcept(f5()) << endl; //1
	cout << noexcept(f6()) << endl; //0
	cout << noexcept(f7()) << endl; //1
	cout << noexcept(f8()) << endl; //1
	return 0;
}