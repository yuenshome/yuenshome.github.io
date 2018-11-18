///观察 容器、迭代器、算法、仿函数之间的关系
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class GreaterN {
public:
	GreaterN(int i=0):val(i){}
	bool operator()(const int& v)const {
		if (v > val) return true;
		return false;
	}
private:
	int val;
};

int main() {
	//vector的定义类似如下：_Alloc是空间配置器,默认值是allocator<_Ty>
	//template<class _Ty,class _Alloc = allocator<_Ty> > 
	//class vector...
	std::vector<int> v1;
	std::vector<int, std::allocator<int>> v2;

	//观察：容器、迭代器、算法、仿函数之间的关系
	//数组也可看作是一种容器
	int arr[5] = { 1,2,3,4,5 };
	int* it1_beg = std::begin(arr); //相当于 &arr[0]
	int* it1_end = std::end(arr);	//相当于 &arr[5]
	//int* 指针,也可看作是一种迭代器
	int total = std::count_if(it1_beg, it1_end, GreaterN(3));
	//GreaterN是函数对象(仿函数),协助算法制定策略
	//count_if是算法,统计在两个迭代器之间所有元素满足条件的个数
	cout << total << endl;

	//vector是一种容器
	std::vector<int> vec = { 1,2,3,4,5 };
	//std::vector<int>::iterator 是一种迭代器
	std::vector<int>::iterator it2_beg = vec.begin();
	std::vector<int>::iterator it2_end = vec.end();
	int total2 = std::count_if(it2_beg, it2_end, GreaterN(2));
	//上面三句可以简写为：
	total2 = std::count_if(vec.begin(), vec.end(), GreaterN(2));
	cout << total2 << endl;

	return 0;
}