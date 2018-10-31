#include <iostream>
using namespace std;

struct Top { int x = 1; };
struct Base : public Top { int y = 2; };
struct D1 :virtual public Base { int i = 3; };
struct D2 :virtual public Base { int j = 4; };
struct M1 :public D1, public D2 { int k = 5; };
struct Ok :public M1, public Top { int n = 6; };

int main() {
	Ok ok;
	//1.写出构造次序
	M1 *p_m1 = &ok;
	D1 *p_d1 = &ok;
	D2 *p_d2 = &ok;
	Base *p_base = &ok;
	//Top *p_top = &d; 
	//2.上面为什么不行?
	//(有两个Top,二义性)
	//3.找一个编译器,尝试画出该情况下的对象内存图

	return 0;
}


