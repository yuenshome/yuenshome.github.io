//copy,copy_if,copy_n,transform
#include <iostream>
#include <vector>
#include <string>
#include <deque>
#include <array>
#include <list>
#include <forward_list>
#include <algorithm>
using namespace std;
class Stu {
public:
	friend ostream& operator<<(ostream&, const Stu&);
	Stu(const string& _name, int _age)
		:name(_name), age(_age) {}
private:
	string name;
	int age;
};
ostream& operator<<(ostream &out, const Stu& s) {
	out << s.name << ":" << s.age;
	return out;
}
int main() {
	//copy(beg,end,dest) dest:存放处理后元素的目标位置
	//将序列中的元素拷贝到dest指定的目标序列中
	//dest下要能放得下那么多元素
	int arr[] = { 11,22,33 };
	vector<int> v0(5); 
	copy(begin(arr), end(arr), v0.begin()); //arr拷贝到v0
	for (const auto &item : v0) cout << item << " "; cout <<endl;
	
	//copy_if(beg,end,dest,unaryPred) unaryPred:一个参数的条件
	//只有符合条件的才拷贝(unaryPred返回true表示符合条件)
	copy_if(begin(arr), end(arr),v0.begin(), //拷贝奇数
				[](const int& t) {if (t % 2 == 1) return 1; return 0; });
	for (const auto &item : v0) cout << item << " "; cout << endl;

	//copy_n(beg,n,dest)
	//复制前n个元素,beg开始的序列中要有n的元素,dest中要能放下n个元素
	vector<int> v00; //用back_inserter
	copy_n(begin(arr), sizeof(arr) / sizeof(int), back_inserter(v00));
	for (const auto &item : v00) cout << item << " "; cout << endl;
	cout << "----------------\n";

	//transform(beg,end,dest,unaryOp) 
	//将序列中的元素调用unaryOp操作,结果存入dest
	list<int> l11 = { 1,2,3,4,5,6 };
	vector<int> v11(10, 0);  //将list中的元素+10后存入vector
	transform(l11.cbegin(), l11.cend(), v11.begin(), //每个元素+10
	/*要保证dest里面的位置足够*/      	[](const int& t) {return t + 10; });
	for (const auto &item : v11) cout << item << " "; cout << endl;

	vector<int> v12;  //利用 back_inserter 插入迭代器实现
	transform(l11.cbegin(), l11.cend(), back_inserter(v12), 
							     	[](const int& t) {return t + 10; });
	for (const auto &item : v12) cout << item << " "; cout << endl;

	//transform(beg,end,beg2,dest,binaryOp)  binaryOp:两个参数的操作
	//将两个序列中的元素作为2个参数传递给binaryOp,操作结果存入dest
	array<int, 3> a13 = { 21,22,23 };
	forward_list<string> f13 = { "张三","李四","王五" };
	deque<Stu> d13;
	transform(a13.cbegin(), a13.cend(), f13.cbegin(), back_inserter(d13), 
			[](const int& age, const string& name) {return Stu{name,age}; });
	for (const auto &item : d13) cout << item << " "; cout << endl;

	return 0;
}
