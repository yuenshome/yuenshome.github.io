#include <iostream>
using namespace std;
int main() {
	//static_cast<转换后类型>(需要转换的数据)
	double fd1 = 1.2;
	int i1 = fd1; //隐式转换,精度丢失,编译器会警告
	i1 = static_cast<int>(fd1); //明确转换,不警告
	void *vi1 = &i1;
	int* pi1 = (int*)vi1; //c写法 void* --> int*
	pi1 = static_cast<int*>(vi1); //c++写法
	//注意：int* <--> char* 不能static_cast
	char *pc1 = (char*)pi1; //c写法 强转int*-->char*
	//pc1 = static_cast<char*>(pi1); 错误

	//const_cast:
	const int i2 = 10;
	const int *cpi2 = &i2;
	int *pi2 = (int*)cpi2; //c写法
	pi2 = const_cast<int*>(cpi2); //去掉底层const
	int &ri2 = const_cast<int&>(i2);

	//reinterpret_cast
	int a = 0x00434241;
	int* p = &a;
	char* pc = (char*)p; //c写法
	pc = reinterpret_cast<char*>(p);
	int *pa = (int*)a; //c写法,int-->int*
	pa = reinterpret_cast<int*>(a);

	return 0;
}