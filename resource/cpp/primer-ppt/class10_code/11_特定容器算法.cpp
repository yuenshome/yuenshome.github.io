//list,forward_list特定算法(成员函数)
#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <forward_list>
#include <algorithm>
#include <functional>
#include <iterator>
using namespace std;

//list,forward_list特定算法(成员函数)
int main() {
	//1. reverse() 反转序列
	forward_list<int> f1 = { 1,2,3,4,5 };
	//reverse(f1.begin(), f1.end()); 不行,需要双向迭代器,list可以但效率不高
	for (const auto& item : f1) { cout << item << " "; } cout << endl;
	f1.reverse(); //反转
	for (const auto& item : f1) { cout << item << " "; } cout << endl;
	cout << "----------------\n";

	//2.remove(val),remove_if(pred) 删除  元素==val, pred条件为真
	//remove(l1.begin(), l1.end(), 2);
	f1.remove(2); //真正删除了元素2
	for (const auto& item : f1) { cout << item << " "; } cout << endl;
	f1 = { 1,2,3,4,5 };
	remove(f1.begin(), f1.end(), 2); //虽然可用,但不是真正删除,效率低
	for (const auto& item : f1) { cout << item << " "; } cout << endl;
	cout << "----------------\n";

	//3.unique(),unique(pred) 去重复, == , pred条件为真 【真正去重,要先排序】
	f1 = { 1,2,2,3,3,3,4,5,5 };
	f1.unique(); //重复的删掉了
	for (const auto& item : f1) { cout << item << " "; } cout << endl;
	cout << "----------------\n";

	//4.sort(),sort(comp)
	//sort(l1.begin(), l1.end()); 不行,sort算法要随机访问迭代器
	f1 = { 3,2,1,2,3,4,9 };
	f1.sort();
	for (const auto& item : f1) { cout << item << " "; } cout << endl;
	f1.sort([](const int& t1, const int& t2) {if (t1 > t2) return 1; return 0; });
	for (const auto& item : f1) { cout << item << " "; } cout << endl; //从大到小
	cout << "----------------\n";

	//5.merge(lst2), merge(lst2,comp) 都是已序的, <运算, comp返回为真
	//合并2个已序序列
	list<int> l1 = { 1,3,5,7,9 }, l2 = { 2,4,6,8,10 }, l3;  //用算法merge: l2保留
	merge(l1.begin(), l1.end(), l2.begin(), l2.end(), back_inserter(l3));
	for (const auto& item : l3) { cout << item << " "; } cout << endl;

	l1 = { 1,3,5,7,9 };	l2 = { 2,4,6,8,10 };
	l1.merge(l2); //l2 内容被清空
	for (const auto& item : l1) { cout << item << " "; } cout << endl;
	for (const auto& item : l2) { cout << item << " "; } cout << endl;
	cout << "----------------\n";

	//6.splice(p,lst2)  splice_after(p,lst2) 
	//将lst2插入到p的位置 (对于forward_list就是p后的位置)
	l1 = { 1,3,5,7,9 };	l2 = { 2,4,6,8,10 };
	l1.splice(++l1.begin(), l2); //将l2插到3的位置,l2内容清空
	for (const auto& item : l1) { cout << item << " "; } cout << endl;
	for (const auto& item : l2) { cout << item << " "; } cout << endl;
	
	forward_list<int> f0 = { 1,3,9,4,5 }, f2 = {13,11,12};
	auto it1 = next(f0.before_begin(), 3); //it1在9的位置
	f0.splice_after(it1, f2);
	for (const auto& item : f0) { cout << item << " "; } cout << endl;
	
	//splice(p,lst2,p2) 将lst2中p2开始的元素移动到 当前列表的p位置
	//splice(p,lst2,b,e) b,e代表lst2中的一个系列范围
	//splice_after(p,lst2,p2)
	//splice_after(p,lst2,b,e)


	return 0;
}