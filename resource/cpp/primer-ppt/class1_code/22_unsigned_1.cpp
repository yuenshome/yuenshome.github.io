#include <iostream>
#include <iomanip>
using namespace std;
int main() {
	//unsigned 注意点：
	unsigned int a = 2, b = 1;
	cout << a - b << endl; // 1
	cout << b - a << endl; // 4294967295
	cout << hex << b - a << dec << endl; //0xFFFFFFFF
	long long long_a = 0xFFFFFFFF;
	cout << long_a << endl;// 4294967295

	//同内存同内容，解释不同而已。
	//相同的地址，相同的内容，只是解释方式的不同
	unsigned int ok = b - a;
	int *p_ok = (int*)&ok;
	cout << *p_ok << "---" << ok << endl;
	cout << &ok << endl;
	cout << p_ok << endl;

	//内存中放置 0x41424344 是啥值？
	char str[5] = { 'A','B','C','D','\0' };
	cout << str << endl;
	int *p_str = (int*)str;
	cout << *p_str << endl; //1145258561
	cout << hex << *p_str << dec << endl;
	cout << hex << (int)(str[0]) << str[1] << str[2] << str[3] << dec << endl;
	//hex输出，可以用 \x?

	//反过来也是一样的
	int test = 1145258561;
	char *p_char = (char*)&test;
	cout << *p_char << *(p_char + 1) << *(p_char + 2) << endl;

	return 0;
}

