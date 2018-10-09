#include <iostream>
using namespace std;
int main() {
	int a = 1, b = 2, c = 3;

	//OK,指针数组
	int *p_arr[] = { &a,&b,&c };

	//错误，在数组中存放引用是不行的
	//int &r_arr[] = { a,b,c }; 

	int arr[3] = { a,b,c };
	int(*p1)[3] = &arr; //ok，数组指针
	int(&r1)[3] = arr;  //ok，数组的引用
	return 0;
}