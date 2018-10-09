#include <iostream>
#include <stdlib.h>
using namespace std;
int main() {
	int *p1 = (int*)malloc(sizeof(int)); 
	free(p1); //C的malloc和free
	int *p2 = new int; //C++：不要类型转换
	delete p2;         //C++：不要sizeof(int)
	//new 和 new[]
	//开辟单变量空间
	int *p3 = new int; //没初始化,随机数
	p3 = new int(10);  //初始化为 10
	int *p4 = new int{100}; //初始化为 100
	//开辟数组空间
	p4 = new int[10];  //开辟10个int空间(没初始化)
	p4 = new int[10]{ 1,2 };//初始化,后面用0初始化
	int **pp = new int*[10]{NULL}; //10个存放int*的空间
	//delete 和 delete []
	delete p3;   // delete 对应 new
	delete[] p4; // delete [] 对应 new []
	delete[] pp;
	return 0;
}