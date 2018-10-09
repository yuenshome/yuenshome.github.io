#include <iostream>
#include <iomanip>
using namespace std;
int main() {
	int a = 12345;
	double f = 123.4567;
	//默认输出整数
	cout << "a=" << a << "===" << endl;
	//占8格，右对齐
	cout << "a=" << setw(8) << a << "===" << endl;
	//占8格，左对齐
	cout << "a=" << setw(8) << setiosflags(ios::left)
		<< a << "===" << endl;
	//默认输出浮点数，有效位数显示6位
	cout << "f=" << f << "===" << endl;
	//占10格，小数显示2位，右对齐
	cout << "f=" << setw(10) << setprecision(2)
		<< setiosflags(ios::fixed) << setiosflags(ios::right)
		<< f << "===" << endl;
	
	return 0;
}

