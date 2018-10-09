#include <iostream>
using namespace std;
int main() {
	int i = 10, j = 20;
	int *pi = &i; //pi是指向i的指针
	cout << *pi << " " << i << endl; //*pi 相当于 i
	*pi = 100;
	cout << *pi << " " << i << endl; //通过指针改变值
	pi = &j; //pi指针指向其他对象了
	cout << *pi << " " << j << endl;

	//空指针：
	//NULL 在c中的定义是 ((void*)0)
	//NULL 在c++中定义是 0
	//nullptr 在c++中用来表示空指针
	int *p = nullptr;
	if (p)
		cout << "p is true" << endl;
	else
		cout << "p is false" << endl;
	return 0;
}

