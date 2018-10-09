#include <iostream>
using namespace std; // 临时量：内置类型是const的，类类型不一定
class myString {
public:
	myString(const char* s = NULL) { cout << "const char* 构造" << endl; }
	myString(const myString &other) { cout << "copy 构造" << endl; }
	myString &operator=(const myString &other) {
		cout << "赋值操作" << endl; return *this; }
	~myString() { cout << "析构" << endl; }
};

myString fun() { myString ss; return ss; }
//myString &fun() {} //返回的是引用
//myString *fun() {} //返回的是指针
int main() {
	myString s1; //const char* 构造
	s1 = fun();  //myString 临时量 = ss;(copy构造) --> s1 = 临时量;(赋值操作)
	//fun(); 
	return 0;
}