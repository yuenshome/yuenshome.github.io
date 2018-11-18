#include <iostream>
#include <cstring>
using namespace std;

//创建静态数组,类型为T,长度为N, 返回其首地址
template<typename T,int N>
T* f1(const T& t) {
	static T tmp[N];
	for (auto &tt : tmp) tt = t;
	return tmp;
}
//比较2个字符串常量  注意：内联声明inline的位置
template<unsigned M,unsigned N>
inline int compare(const char(&p1)[M], const char(&p2)[N]) {
	cout << "M=" << M << ",N=" << N << endl;
	return strcmp(p1, p2);
}
int main() {
	int *p = f1<int,10>(33);
	for (int i = 0; i < 10; i++)
		cout << *p++ << " ";
	cout << endl;
	int flag = compare("abc","abcd");
	return 0;
}