#include <iostream>
using namespace std;
int main() {
	int i = 10, j = 30;
	int *p1 = &i; //无const限定
	*p1 = 20; //可以改变指向变量的值(i-->20)
	p1 = &j;  //可以改变指向的变量(p1指向了j)

	//指向常量的指针
	const int *p2; //const在*前面(也可写int const *p2)
	p2 = &i;	//p2 的指向 可以改变 (意味着p2不是常量)
	p2 = &j;
	//*p2 = 100; 错误,*p2改不了值(意味着*p2是常量)

	//常量指针(指针本身是常量)
	int * const p3 = &i; //const在*后面
	//p3 = &j; 错误,p3的指向不能改变(p3是常量)
	*p3 = 100; //OK, *p3可以修改

	//指向常量的常量指针
	const int * const p4 = &i; //两个const
	//p4 = &j; 错误
	//*p4 = 100; 错误
	return 0;
}