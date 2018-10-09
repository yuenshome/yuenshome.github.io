#include <iostream>
using namespace std;
int main() {
	//int 和 bool 转换 (bool: 1,0)
	int i = 3;
	bool b = true;
	i = b; //bool转int
	cout << "i= " << i << endl; //i=1
	cout << "b= " << b << endl; //b=1
	i = 3;
	b = i;
	cout << "b= " << b << endl; //b=1
	b = 0; //int转bool
	cout << "b= " << b << endl; //b=0

	i = 3; b = true;
	cout << i + b << endl; //4 (bool先转int再运算)

	i = 3; b = 1;
	cout << (b == i) << (i == b) << endl; //0 0 bool先转int再比较
	i = 1; b = 1;
	cout << (i == b) << endl; //1 bool先转int再比较

	long long_i = 123L;
	long long llong_i = 1234LL;
	int hex1 = 0xAA;
	int oct1 = 017;
	int dec = 18;
	return 0;
}

