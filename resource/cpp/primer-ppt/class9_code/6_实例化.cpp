#include <iostream>
#include <vector>
#include <list>
#include <string>
using namespace std;

//模仿std::find,在两个迭代器之间找到匹配的元素,返回迭代器
//T为迭代器类型,V为元素类型
template<typename T,typename V>
T my_find(const T& b,const T& e,const V& value) {
	T it = b;
	while (it != e && *it != value)
		++it;
	return it;
}

//编写my_begin,my_end,返回数组的头尾指针
template<typename T,unsigned N>
T* my_begin(T(&arr)[N]) {
	cout << "size: " << N << endl;
	return &arr[0];
}
template<typename T,unsigned N>
T* my_end(T(&arr)[N]) {
	return &arr[N - 1];
}

//实例化的结果：
//vector<int>::iterator my_find(const vector<int>::iterator& b, const vector<int>::iterator& e, const int& value);
//list<string>::iterator my_find(const list<string>::iterator& b, const list<string>::iterator& e, const string& value);
//int* my_begin(int(&arr)[5]);
//int* my_end(int(&arr)[5]);

int main() {
	vector<int> vec = { 1,4,7,2,5,8 };
	vector<int>::iterator it1 = my_find(vec.begin(),vec.end(),2);
	
	if (it1 != vec.end()) 
		cout << "找到了: " << *it1 << endl;
	else 
		cout << "vec中没有找到\n";
	list<string> ls = { "abc","C++","C#" };
	list<string>::iterator it2 = my_find(ls.begin(), ls.end(), "C++");
	if (it2 != ls.end())
		cout << "找到了: " << *it2 << endl;
	else
		cout << "list中没有找到\n";
	cout << "-------------\n";
	int arr_int[] = { 1,3,5,7,9 };
	int * p_int = my_find(my_begin(arr_int), my_end(arr_int),5);
	if (p_int != my_end(arr_int))
		cout << "找到了: " << *p_int << endl;
	else 
		cout << "arr_int中没有找到\n";
	p_int = my_find(my_begin(arr_int), my_end(arr_int), 6);
	if (p_int != my_end(arr_int))
		cout << "找到了: " << *p_int << endl;
	else
		cout << "arr_int中没有找到\n";
	cout << "-------------\n";
	p_int = std::find(std::begin(arr_int), std::end(arr_int), 6);
	if (p_int != std::end(arr_int))  
		cout << "找到了: " << *p_int << endl;
	else
		cout << "arr_int中没有找到\n";
	return 0;
}