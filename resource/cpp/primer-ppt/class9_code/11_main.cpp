/// 11_main.cpp
#include <iostream>
#include "11_template.h"
using namespace std;

//实例化声明
extern template int compare(const int&, const int&);
extern template class my_vector<int>;

struct A{ A(int i){} };
//template class my_vector<A>; 
//错误,A没有默认构造,无法实例化my_vector<A>的默认构造

int main() {
	int a = 10, b = 20;
	cout << compare(a, b) << endl; //实例化在其他地方
	my_vector<int> v1;  //实例化在其他地方
	//my_vector<double> 和 带参构造函数 在本文件实例化
	my_vector<double> v2(1); 
	//my_vector<A> 和 带参构造函数 在本文件实例化
	my_vector<A> v3(A(1),3); 
	return 0;
}