#include <iostream>
using namespace std;
int main() {
	const int i = 10;
	//int &ri = i; 错误,非常量引用指向常量
	int ii = 20;
	const int &rii = ii; //OK
	//rii = 30; 错误，常量引用无法修改值

	double fd = 1.23;
	//int &r = fd; 错误
	const int &r = fd; //OK
   	 //观察 fd 和 r 的值
	cout << fd << " " << r << endl;
	//观察 fd 和 r 的地址
	cout << &fd << endl;
	cout << &r << endl;

	return 0;
}