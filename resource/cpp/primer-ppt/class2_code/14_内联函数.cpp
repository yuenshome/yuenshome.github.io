#include <iostream>
using namespace std;
//C语言做法
#define max_val_c(a,b) (((a) > (b)) ? (a) : (b))
//C++做法,inline内联函数
inline int max_val(int a, int b) {
	return a > b ? a : b;
}

int main() {
	int a1 = 10, a2 = 20;
	int c = max_val_c(a1,a2); //预编译时展开
	int cpp = max_val(a1, a2);	//编译器在编译过程中转变为 int cpp = a1 > a2 ? a1 : a2;
	return 0;
}