#include <iostream>
using namespace std;
void f1(int a){}
void f1(float a){}
void f2(long a) {}
void f2(double a){}
int main() {
	//f1(1.2); 
	//二义性：1.2是double类型,向int和float都可转换
	//f2(1); 
	//二义性: 1是整数,向long和double都可转换
	return 0;
}