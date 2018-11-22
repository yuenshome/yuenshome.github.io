//查找find,find_if,count,count_if
#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <array>
#include <algorithm>
using namespace std;
struct cmp1 {
	cmp1(int th=0):thresh(th){}
	bool operator()(const int& t) {
		if (t > thresh) return true;
		return false;
	}
	int thresh;
};
struct cmp2 {
	cmp2(double th = 0) :thresh(th) {}
	bool operator()(const double& t) {
		if (t == thresh) return true;
		return false;
	}
	double thresh;
};

int main() {
	vector<int> v1 = { 2,1,2,4,7, };
	//find(beg,end,val) beg:迭代器起始位置,end:迭代器结束位置,val:要找的值
	//在迭代器范围内,找到该元素,则: 返回第一个该元素的迭代器位置
	vector<int>::const_iterator res1 = find(v1.cbegin(), v1.cend(), 1);
	if (res1 != v1.end()) 
		cout << "元素1:找到," << *res1 << endl;
	//若没有找到该元素,则: 返回end位置的迭代器
	vector<int>::iterator res2 = find(v1.begin() + 3, v1.end() - 1, 10);
	if (res2 == v1.end() - 1) //vector迭代器是随机迭代器,可以+ -
		cout << "元素10:没有找到\n";

	//find_if(beg,end,unaryPred) beg:迭代器起始位置,end:结束位置,unaryPred:条件
	//在迭代器范围内,找符合条件unaryPred的元素, 找到、未找到返回同find()
	//unaryPred: 一元谓词,接受一个参数(就是迭代器所指向的元素),返回可作为条件的类型
	auto res3 = find_if(v1.cbegin(), v1.cend(), cmp1(4)); //找>4的元素
	if (res3 != v1.cend()) 
		cout << ">4的元素找到," << *res3 << endl;
	auto res4 = find_if(v1.begin(), v1.end(), cmp1(10)); //找>10的元素
	if (res3 == v1.end())
		cout << ">10的元素没有找到\n";

	//count(beg,end,val) 
	//在迭代器范围内,找出元素值==val 的元素个数,返回值等于val的元素个数
	list<double> l1 = { 1.0,2.0,1.0,3.0 };
	cout << count(l1.begin(), l1.end(), 1.0) << endl; // 2
	cout << count(l1.begin(), l1.end(), 4.0) << endl; // 0

	//count_if(beg,end,unaryPred) 
	//在迭代器范围内,统计符合条件unaryPred的元素个数
	cout << count_if(l1.cbegin(), l1.cend(), cmp2(1.0)) << endl; //==1.0的个数 2

	return 0;
}