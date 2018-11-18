///迭代器 初步
#include <iostream>
#include <vector>
using namespace std;

int main() {
	vector<int> v1 = { 1,2,3 };
	//迭代器
	vector<int>::iterator it1 = v1.begin(); //end();
	//const迭代器
	vector<int>::const_iterator it1_c = v1.cbegin(); //cend();
	//反向迭代器
	vector<int>::reverse_iterator it1_r = v1.rbegin(); //rend();
	//反向const迭代器
	vector<int>::const_reverse_iterator it1_cr = v1.crbegin(); //crend();

	//成员函数begin/rbegin/end/rend是重载过的
	const vector<int> v2 = { 1,2,3 };
	auto it11 = v1.begin(); //it11 是 vector<int>::iterator
	auto it22 = v2.begin(); //it22 是 vector<int>::const_iterator
	*it11 = 11; //解引用访问
	//*it22 = 10; //错,const迭代器 只读
	cout << *it22 << endl; //能读不能写

	//begin 和 end
	//容器有成员函数begin/end等...
	//也可以用std::begin(xx),对内置数组也可使用 end对应
	it1 = std::begin(v1);
	it1_c = std::cbegin(v1);
	it1_r = std::rbegin(v1);
	it1_cr = std::crbegin(v1);
	
	//迭代器范围 [begin, end) 左闭右开
	while (it1_c != cend(v1)) {
		cout << *it1_c << " ";
		++it1_c;
	}
	cout << endl;
	//反向迭代器,++是从后面向前
	while (it1_cr != crend(v1)) {
		cout << *it1_cr << " ";
		++it1_cr;
	}
	cout << endl;

	return 0;
}