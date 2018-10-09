#include <iostream>
using namespace std;
int main() {
	int a = 1, b = 1;
	(a = 0) && (b = 100); //&&左侧表达式为假，右侧不运算
	cout << a << b << endl; // a=0 b=1
	int i = 1, j = 1;
	(i = 10) || (j = 100); //||左侧表达式为真，右侧不运算
	cout << i << j << endl; //i=10 j=1

	//注意次序：
	int c = 1;
	//a<b先比,返回bool类型,然后bool类型提升为int(0或1)和c比较
	bool b1 = a < b < c; 
	
	//注意优先级：先b<c比较得到bool类型,提升为int再和a==判断
	a == b < c;

	//注意bool类型：
	a = 10;
	while (a)  //循环10次
		a--;
	a = 10;
	while (a == true)  //循环0次
		a--;
	return 0;
}