#include <iostream>
#include <vector>
using namespace std;
class B {};
class A{
public:
	A(int i=0):data(i){ }					//int -> A   
	explicit A(const B&) { /*...*/ }		//B -->  A   
	operator int()const { return data; }    //A -->int  
	explicit operator B()const{return B();} //A -->  B 
	operator bool()const {	return data; }  //A --> bool
private:
	int data;
};

int main() {
	//1.1 没有explicit修饰的构造函数,参数类型 ==> 本类类型
	A a1(1);   // int -> A
	A a2 = 2;  // int -> A
	//1.2 有explicit修饰的构造函数,参数类型 ==> 本类类型
	B b;
	A a3(b);   // B -> A
	//A a4 = b;  //因为 explicit,隐式转换不行
	A a5 = static_cast<A>(b); //显式转换ok

	//2.1 没有explicit修饰的类型转换运算符, 本类类型 ==> 其他类型
	A a;
	int i1(a); // A -> int 
	int i2 = a;// A -> int
	//2.2 有explicit修饰的类型转换运算符, 本类类型 ==> 其他类型
	A aa;
	B b1(aa);    // A -> B
	//B b2 = aa; // 因为explicit,隐式转换不行
	B b3 = static_cast<B>(aa); //显示转换ok

	//转换为bool类型
	if ( A(1) ) cout << "true\n";
	if ( !A() ) cout << "false\n";

	return 0;
}