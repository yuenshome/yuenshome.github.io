#include <iostream>
using namespace std;
int main() {
	//int 和 char 转换
	int i = 67;
	char c = 'A'; // 'A'的ascii码是 65
	c = i;
	cout << "c= " << c << endl; //c= C
	c = 'A'; i = c;
	cout << "i= " << i << endl; //i= 65
	c = 'A'; i = 66;
	cout << c + i << endl;//131 (运算时，char先转int)
	cout << (i > c) << endl;//1

	return 0;
}

