#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

class Func {
public:
	Func(int len = 3) :length(len) {}
	bool operator()(const string& s) {
		if (s.size() < length) return true;
		return false;
	}
private:
	int length;
};

bool less3(const string& s) {
	if (s.size() < 3) return true;
	return false;
}
int main() {
	vector<string> vs = { "abc","as","a","aff" };
	int res = count_if(vs.begin(), vs.end(), less3);
	cout << res << endl; //输出 2

	//问题：假如要求输出长度小于4的元素个数怎么办？
	//方案1：修改less3函数，里面的 3修改为4（不方便，太麻烦）
	//方案2：less3函数增加一个参数，但是count_if函数中的第3个参数是函数指针，
	//		只允许有一个参数，（不行）
	//方案3：搞一个全局变量，less3函数中 的 3用全局变量替代（不方便,容易出错）

	//仿函数：通过重载 operator() 函数调用运算符
	res = count_if(vs.begin(), vs.end(), Func()); //默认构造是3,注意这里的()
	cout << res << endl; //输出2
	res = count_if(vs.begin(), vs.end(), Func(4));
	cout << res << endl; //输出4
	return 0;
}
