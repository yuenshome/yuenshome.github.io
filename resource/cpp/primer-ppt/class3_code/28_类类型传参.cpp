#include <iostream>
using namespace std; // 临时量：内置类型是const的，类类型不一定
class myString {
public:
	myString(const char* s = NULL) { cout << "const char* 构造" << endl; }
	myString(const myString &other) { cout << "copy 构造" << endl; }
	myString &operator=(const myString &other) { cout << "赋值操作" << endl; }
	~myString() { cout << "析构" << endl; }
};
void fun(myString ss){}
//void fun(myString &ss) {} //引用参数，相当于 myString &ss = s1; 没有开辟内存
//void fun(myString *ss) {} //指针参数，只是复制了4字节的指针数据
int main() {
	myString s1 = "abc"; // 语义是：先隐式转换，然后copy构造。
						 // 编译器优化：直接 const char* 构造
	fun(s1); //copy构造： 语义： myString ss = s1;
	fun("abc"); //语义：myString ss = "abc"; 与s1构造类似
	return 0;
}