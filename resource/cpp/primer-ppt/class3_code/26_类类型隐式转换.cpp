#include <iostream>
using namespace std;
class A{};
class myString {
public:
	myString(const char* s = NULL) { cout << "myString(const char* s)" << endl; }
	myString(int size, char c = ' ') { cout << "构建size个c组成的字符串" << endl; }
	explicit myString(A a) { cout << "explicit myString(A a)" << endl; }
	myString(const myString& other) { cout << "copy初始化" << endl; }
	myString &operator=(const myString& other) { cout << "赋值函数" << endl; return *this; }
};

int main() {
	//int a{ 1.2 }; //错误,列表初始化
	//观察下面的隐式类型转换
	int a1(1.2);  //直接初始化
	int a2 = 1.2; //拷贝初始化
	int a3;
	a3 = 1.2;     //赋值

	//自己定义的类如何实现这些？
	myString s1("abc"); //直接初始化
	cout << "--------" << endl;
	//拷贝初始化[拷贝前先隐式类型转换](编译器可能会优化)
	myString s2 = "abc"; 
	cout << "========" << endl;
	s2 = "abc"; //先隐式转换为 myString,再 赋值
	//调用 myString(const char*) 函数进行 隐式转换
	cout << "--------" << endl;
	myString s3 = 20; //这样，也行！
	//调用了 myString(int size,char c=' ')
	//实际上，是创建一个 20个空格组成的字符串
	//但是，我们看起来：是 数字20 赋值 给了 myString
	//容易引起混乱，所以这样的构造函数，前面要加 explicit
	cout << "========" << endl;
	A a;
	//s3 = a; //错误，由于explicit修饰，A类型无法隐式转换为myString
	s3 = myString(a); 
	s3 = (myString)a;			   //只能显式地转换
	s3 = static_cast<myString>(a); //只能显式地转换
	cout << "--------" << endl;

	// 要实现 A -> B 的隐式转换，B中必须有非explicit构造函数,参数是A
	// 1. explicit: 抑制构造函数定义的隐式转换
	// 2. explicit构造函数，只能用于直接初始化
	// 3. explicit关键字：只能在类内声明使用，类外不能加

	// 不能用explicit构造函数来实现 myString s = xxx;这样的拷贝构造
	// 也就是说：假如 explicit myString(const char* s = NULL)
	// 那么：myString S("abc"); //ok  myString S = "abc"; //error

	return 0;
}