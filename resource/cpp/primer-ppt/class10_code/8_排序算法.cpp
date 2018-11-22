//sort,stable_sort,partial_sort,nth_element
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
	//sort(beg,end) 排序,要求随机访问迭代器,元素有<运算,从小到大
	//sort(beg,end,comp) 用comp来确定如何排序
	vector<int> v1{ 2,1,3,4,2,8,9,0 };
	sort(v1.begin(), v1.end());
	for (const auto& item : v1) cout << item << " "; cout << endl;
	sort(v1.begin(), v1.end(), greater<int>());
	for (const auto& item : v1) cout << item << " "; cout << endl;
	sort(v1.begin(), v1.end(), //奇数在前,偶数在后
	  [](const int& t1, const int& t2){if(t2%2 < t1%2)return 1;return 0;});
	for (const auto& item : v1) cout << item << " "; cout << endl;
	cout << "----------------\n";

	//stable_sort(beg,end) 稳定排序,随机访问迭代器, <运算
	//stable_sort(beg,end,comp)
	stable_sort(v1.begin(), v1.end());
	for (const auto& item : v1) cout << item << " "; cout << endl;
	cout << "----------------\n";

	//partial_sort(beg,mid,end) 排序mid-beg个元素,相当于找出前几个最小元素
	//partial_sort(beg,mid,end,comp)
	deque<int> d1 = { 9,8,1,3,2,1 };
	partial_sort(d1.begin(), d1.begin() + 4, d1.end()); // 1 1 2 3...
	for (const auto& item : d1) cout << item << " "; cout << endl;
	partial_sort(d1.begin(),d1.begin()+4,d1.end(),greater<int>());//9 8 3...
	for (const auto& item : d1) cout << item << " "; cout << endl;
	cout << "----------------\n";

	//nth_element(beg,nth,end) nth是迭代器位置(指向一个元素),
	//排序完后,nth指向还是那个元素,迭代器之前的都小于等于它,迭代器之后的都大于等于它
	//nth_element(beg,nth,end,comp)
	array<int, 7> a1 = { 1,3,8,2,5,4,9 };
	auto it1 = a1.begin() + 4; //指向元素 5
	nth_element(a1.begin(), it1, a1.end());
	cout << "it1: " << *it1 << endl;
	for (const auto& item : a1) cout << item << " "; cout << endl;

	return 0;
}
