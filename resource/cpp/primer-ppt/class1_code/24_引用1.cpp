#include <iostream>
using namespace std;
int main() {
	int i = 100;
	int &ref_i = i;	//ok
	//int &ref_i2;	//错误(引用必须被初始化)

	//注意书写方式：
	int *a1 = nullptr, a2 = 0;
	//上面一行：a1是指针,a2是int变量
	int &r1 = a2, r2 = a2;
	//上面一行：r1是a2的引用，r2是int变量
	return 0;
}

