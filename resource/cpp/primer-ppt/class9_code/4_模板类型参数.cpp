#include <iostream>
using namespace std;
class A {};
class B {
public:
	bool operator<(const B& rhs)const {
		//...
		return true;
	}
};
//调用myswap的类型T必须支持拷贝构造和赋值
template<typename T>
void myswap(T& a, T& b) {
	T tmp = a;
	a = b;
	b = tmp;
}
//调用compare的类型T必须支持 < 操作
template<typename T>
int compare(const T& a, const T& b) {
	if (a < b) return -1;
	if (b < a) return 1;
	return 0;
}
//参数类型,可以作为返回类型使用
//可以值返回(包括指针返回),引用返回
template<typename T1,typename T2>
T1 *f1(T1& t1, T2& t2) { 
	//...
	return &t1;
};
int main() {
	A a1, a2; //类类型
	myswap(a1, a2);
	myswap<A>(a1, a2);

	//compare(a1, a2); 错(A没有<操作)
	B b1, b2;
	compare(b1, b2);

	int i1 = 1, i2 = 2;
	f1(i1, i2); //T1 T2 都是 int类型
	double d1 = 1.0;
	f1(i1, d1); //T1 int,T2 double
	return 0;
}