#include <iostream>
#include <string>
using namespace std;

void fun(int &a) {}
void func(const int &a) {}
bool find(string &s, char c) {
	return true; //s能在这里修改
}
bool findc(const string &s, char c) {
	return true; //保证s在这里不能修改
}
int main() {
	//fun(42); 不行
	func(42); //ok
	const string s1 = "abc";
	//find(s1, 'c'); //不行
	findc(s1, 'c'); //ok
	return 0;
}