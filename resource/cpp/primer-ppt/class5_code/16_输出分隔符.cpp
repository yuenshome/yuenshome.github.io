#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
class PrintString {
public:
	PrintString(char c = ' ') :sep(c) {}
	void operator()(const string& s)const {
		cout << s << sep;
	}
private:
	char sep;
};
int main() {
	vector<string> vs = { "abc","as","a","aff" };
	//for_each 函数 的第三个参数 也是一个函数指针，只允许有一个参数
	for_each(vs.begin(), vs.end(), PrintString());//默认分隔符
	cout << endl;
	for_each(vs.begin(), vs.end(), PrintString(','));//分隔符是,
	cout << endl;
	PrintString fps1 = PrintString('|');
	for_each(vs.begin(), vs.end(), fps1); //分隔符是|
	cout << endl;
	return 0;
}