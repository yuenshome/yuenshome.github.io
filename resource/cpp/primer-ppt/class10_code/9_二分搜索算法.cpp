///二分搜索
//binary_search,lower_bound,upper_bound,equal_range
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
	//binary_search(beg,end,val) 在已序序列上搜索是否有元素值==val
	//有返回true,没有返回false  【必须有 < (==) 运算】
	//binary_search(beg,end,val,comp)
	vector<int> v1{ 20,10,30,40,20,80,90,0 };
	sort(v1.begin(), v1.end());
	bool flag = binary_search(v1.begin(), v1.end(), 31);
	cout << flag << endl;
	flag = binary_search(v1.begin(), v1.end(), 31, //误差+-1认为相等
		[](const int&t1, const int&t2){if(t1+1==t2||t1-1==t2)return 1;return 0;});
	cout << flag << endl; //31被找到了。
	cout << "---------------\n";

	//lower_bound(beg,end,val) 在已序序列上搜索元素==val,没有搜到返回end
	//找到，返回一个迭代器: 指向 第一个 大于等于 val 的元素  【<运算】
	//lower_bound(beg,end,val,comp)
	v1 = { 1,2,3,4,4,4,5,6 };
	auto it1 = lower_bound(v1.begin(), v1.end(), 3); //找3,返回3的位置
	cout << "找3: " << *it1 << endl;
	it1 = lower_bound(v1.begin(), v1.end(), 4); //找4,返回第1个4的位置
	for_each(it1, v1.end(), [](const int& t) {cout << t << " "; });
	cout << endl <<"---------------\n";

	//upper_bound(beg,end,val) 在已序序列上搜索元素==val,没有搜到返回end
	//找到，返回一个迭代器：指向 第一个 大于 val 的元素
	//upper_bound(beg,end,val,comp)
	auto it2 = upper_bound(v1.cbegin(), v1.cend(), 3); //找3，返回第1个4的位置
	for_each(it2, v1.cend(), [](const int& t) {cout << t << " "; });
	cout << endl;
	it2 = upper_bound(v1.cbegin(), v1.cend(), 4); //找4，返回第5的位置
	for_each(it2, v1.cend(), [](const int& t) {cout << t << " "; });
	cout << endl << "---------------\n";

	//equal_range(beg,end,val) 在已序序列中找元素==val, 返回一对pair, 
	//first成员是lower_bound返回的迭代器,second成员是upper_bound返回的迭代器
	//equal_range(beg,end,val,comp)
	for (const auto& item : v1) cout << item << " "; cout << endl;
	pair<vector<int>::iterator, vector<int>::iterator> pr1 =
		equal_range(v1.begin(), v1.end(), 4);
	cout << "*first: " << *pr1.first << endl;
	cout << "*second: " << *pr1.second << endl;
	for_each(pr1.first, pr1.second, [](const int &t) {cout << t << " "; });
	cout << endl;

	return 0;
}