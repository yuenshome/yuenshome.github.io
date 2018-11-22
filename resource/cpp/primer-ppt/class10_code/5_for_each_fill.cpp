//for_each, fill,fill_n (插入迭代器)
#include <iostream>
#include <vector>
#include <string>
#include <forward_list>
#include <deque>
#include <array>
#include <algorithm>
using namespace std;

class Stu {
public:
	friend ostream& operator<<(ostream&, const Stu&);
	Stu(const string& _name,int _age)
		:name(_name),age(_age){}
private:
	string name;
	int age;
};
ostream& operator<<(ostream &out,const Stu& s) {
	out << s.name << ":" << s.age;
	return out;
}


int main() {
	//for_each(beg,end,unaryOp) unaryOp:一元操作
	//对迭代器beg到end之间的每个元素执行 unaryOp操作
	forward_list<int> f1 = { 1,2,3 };
	for_each(f1.cbegin(), f1.cend(), //打印输出
		[](const int& item) {cout << item << " -> "; });
	cout <<"NULL"<< endl;
	//如果迭代器允许通过解引用改变元素值,unaryOp也可修改元素值
	for_each(f1.begin(), f1.end(), [](int &item) {if (item > 1)item+=10; });
	for (const auto& item : f1)
		cout << item << " -> ";
	cout << "NULL" << endl;

	//fill(beg,end,val)
	//将迭代器范围beg到end中的元素值 设置为val
	deque<Stu> d1 = { {"张三",21},{"李四",22},{"王五",20} };
	for (const auto& item : d1) cout << item << " "; cout << endl;
	fill(d1.begin(), d1.end(), Stu("NULL", 0)); 
	for (const auto& item : d1) cout << item << " "; cout << endl;

	//fill_n(beg,cnt,val) cnt个数
	//从迭代器beg位置开始,设置cnt个元素的值为val,返回最后一个设置元素后面位置的迭代器
	vector<int> v1 = { 1,2,3,4,5,6,7 };
	auto res1 = fill_n(v1.begin(), 3, 0); //从开始位置,设置3个0
	cout << *res1 << endl; //4
	for (const auto& item : v1) cout << item << " "; cout <<endl;
	//fill_n(v1.begin(), 10, 1); 错误,v1没有10个元素,程序员自己保证

	//back_inserter: 插入迭代器(迭代器适配器)	使用push_back的迭代器
	back_insert_iterator<vector<int>> it_back = back_inserter(v1);
	fill_n(it_back, 3, 11); //v1后面增加3个11(调用push_back)
	for (const auto& item : v1) cout << item << " "; cout << endl;

	//front_inserter: 插入迭代器(迭代器适配器) 使用push_front的迭代器
	//fill_n(front_inserter(v1), 2, 33); 错,vector没有push_front
	fill_n(front_inserter(d1), 2, Stu("王五", 20)); //调用push_front
	for (const auto& item : d1) cout << item << " "; cout << endl;

	//inserter: 插入迭代器(迭代器适配器) 在给出的迭代器位置前插入
	insert_iterator<vector<int>> it_insert = inserter(v1, v1.end());
	fill_n(it_insert, 3, 22);
	for (const auto& item : v1) cout << item << " "; cout << endl;
	fill_n(inserter(v1, v1.begin()), 3, -1); 
	for (const auto& item : v1) cout << item << " "; cout << endl;

	return 0;
}