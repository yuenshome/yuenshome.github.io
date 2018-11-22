//remove,remove_if,unique,reverse,random_shuffle
#include <iostream>
#include <vector>
#include <string>
#include <deque>
#include <array>
#include <list>
#include <forward_list>
#include <algorithm>
#include <functional>
using namespace std;

int main() {
	//remove(beg,end,val) 删除值是val的元素   ==运算 
	//算法不会改变容器的大小,采用的办法是:用保留的元素覆盖要删除的元素
	//返回一个迭代器,指向最后一个删除元素的后面的位置
	array<int, 5> a11 = { 1,2,3,1,4 };
	auto it1 = remove(a11.begin(), a11.end(), 1);
	cout << *it1 << endl;
	for (const auto &item : a11) cout << item << " "; cout << endl;

	//remove_if(beg,end,unaryPred) 根据unaryPred条件决定是否删除
	forward_list<int> f11 = { 1,3,2,4,1 };
	auto it2 = remove_if(f11.begin(), f11.end(), //偶数删除
		[](const int& t) {if (t % 2 == 0) return true; return false; });
	cout << *it2 << endl;
	for (const auto &item : f11) cout << item << " "; cout << endl;
	cout << "--------------\n";

	//unique(beg,end) 对相邻元素,假如重复,会通过“覆盖”来删除(类似remove)
	//返回一个迭代器,指向不重复元素的尾后位置   ==运算
	//unique(beg,end,binaryPred)
	array<int, 7> a1 = { 1,2,3,1,4,4,2 };
	//不排序去重复：只会去除相邻的
	for (const auto &item : a1) cout << item << " "; cout << endl;
	auto it3 = unique(a1.begin(), a1.end());
	for_each(a1.begin(), it3, [](const int& t) {cout << t << " "; });
	cout << endl << "--------------\n";

	//排序以后，再unique:
	a1 = { 1,2,3,1,4,4,2 };
	sort(a1.begin(), a1.end());
	for (const auto &item : a1) cout << item << " "; cout << endl;
	auto it4 = unique(a1.begin(), a1.end());
	for_each(a1.begin(), it4, [](const int& t) {cout << t << " "; });
	cout << endl << "--------------\n";

	//reverse(beg,end) 翻转序列中的元素,需要双向迭代器
	a1 = { 1,2,3,4,5,6,7 };
	for (const auto &item : a1) cout << item << " "; cout << endl;
	reverse(a1.begin(), a1.end());
	for (const auto &item : a1) cout << item << " "; cout << endl;
	cout << endl << "--------------\n";

	//random_shuffle(beg,end) 打乱序列 需要随机访问迭代器
	random_shuffle(a1.begin(), a1.end());
	for (const auto &item : a1) cout << item << " "; cout << endl;
	random_shuffle(a1.begin(), a1.end());
	for (const auto &item : a1) cout << item << " "; cout << endl;

	return 0;
}