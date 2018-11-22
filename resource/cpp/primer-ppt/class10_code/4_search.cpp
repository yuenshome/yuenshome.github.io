///查找子序列 search
//search
#include <iostream>
#include <vector>
#include <string>
#include <deque>
#include <array>
#include <algorithm>
using namespace std;

bool cmp3(const int& a, const int& b) {
	//只要都是奇数 或 偶数 就认为相等
	if (a % 2 == b % 2) return true;
	return false;
}

int main() {
	//search(beg1,end1,beg2,end2) beg1,end1和beg2,end2表示2对迭代器范围
	//返回 beg2到end2(子序列) 在beg1到end1中间第1次出现的位置,没有找到返回end1
	int arr1[] = { 1,2,3,5,8,2,3,5 };
	int arr2[] = { 2,3,5 }, arr3[] = { 1,2,4 };
	auto res5 = search(begin(arr1), end(arr1), begin(arr2), end(arr2));
	if (res5 != end(arr1))
		cout << "找到子序列2,3,5: " << *res5 << endl; //*res = 2
	auto res6 = search(cbegin(arr1), cend(arr1), &arr3[1], &arr3[3]);
	if (res6 == cend(arr1))
		cout << "没有找到子序列2,4\n";

	//search(beg1,end1,beg2,end2,binaryPred)
	//binaryPred: 二元谓词,接受两个参数(两个元素的比较),返回可作为条件的类型
	//查找beg2到end2子序列 在beg1到end1中间 出现的位置,判断条件用 binaryPred
	array<int, 5> a1 = { 1,8,3,3,4 }; //奇 偶 奇 奇 偶
	array<int, 4> a2 = { 3,4,1,5 }, a3 = { 4,1,3,2 }, a4 = { 1,1,2,1 };
	auto res7 = search(a1.begin(), a1.end(), a2.begin(), a2.end(), cmp3);
	if (res7 != a1.end())
		cout << "3,4,1,5子序列找到: " << *res7 << endl;
	auto res8 = search(a1.begin(), a1.end(), a3.begin(), a3.end(), cmp3);
	if (res8 != a1.end())
		cout << "4,1,3,2子序列找到: " << *res8 << endl;
	auto res9 = search(a1.begin(), a1.end(), a4.begin(), a4.end(), cmp3);
	if (res9 == a1.end())
		cout << "1,1,2,1子序列没找到\n";
	auto res10 = search(a1.cbegin(), a1.cend(), a4.crbegin(), a4.crend(), cmp3);
	if (res10 != a1.end())
		cout << "1,1,2,1的反向子序列找到: " << *res10 << endl;

	return 0;
}