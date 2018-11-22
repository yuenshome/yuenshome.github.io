///unordered_set,unordered_map等
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>

using namespace std;

int main() {
	//0.set和unordered_set的比较(map和unordered_map也是一样)：
	set<int> set1 = { 3,2,1,2,4,6 };
	unordered_set<int> uset1 = { 3,2,1,2,4,6 };
	unordered_multiset<int> umset1 = { 3,2,1,2,4,6 };
	for (auto& item : set1) cout << item << " "; cout << endl; //1 2 3 4 6 有序
	for (auto& item : uset1) cout << item << " "; cout << endl;//3 2 1 4 6 无序
	for (auto& item : umset1) cout << item << " "; cout << endl;//3 2 2 1 4 6 无序
	//(unordered_multiset：相等的元素，会放在一起)
	cout << "------------------------\n";

	//1.初始化
	unordered_map<string, int, hash<string>> g1;//默认就是 hash<string>
	unordered_multimap<int, double> g2 = { {1,1.1},{2,2.2} };
	unordered_map<string, int> g3{ { "C++",80 },{ "Python",90 },{ "C",85 } };
	unordered_map<string, int> g4(g3); 
	unordered_map<string, int> g5 = g3; //拷贝构造
	unordered_map<string, int> g7(g3.begin(), g3.end()); //迭代器范围初始化
	
	//2.赋值,swap
	g4 = g3;
	unordered_multimap<string, int> g8;
	g8 = { { "C++",80 },{ "Python",90 },{ "C",85 },{ "C",85 } };
	g4.swap(g3); std::swap(g3, g4);

	//3.容量相关
	g7.size();   g7.empty();   g7.max_size();
	cout << "g7中桶的数量: "<<g7.bucket_count()<<endl; //当前桶数量
	g7.max_bucket_count(); //容器能容纳的最多的桶数量
	for (int i = 0; i < g7.bucket_count(); i++) //第几个桶中元素的个数
		cout << "第" << i << "个桶中元素数量: " << g7.bucket_size(i) << endl;
	cout << "\"C++\"在桶: "<<g7.bucket("C++") << endl;//查找某个关键字在哪个桶中
	cout << "------------------------\n";
	
	//4.哈希策略
	cout << "每个桶的平均元素数量(double): " << g7.load_factor() << endl;
	cout << g7.max_load_factor() << endl; 
	//c++试图维护的平均桶大小,需要时会添加新的桶,使得 load_factor<=max_load_factor
	g7.rehash(10); //重组存储,使得 bucket_count(桶数) >= n
	g7.reserve(100); //重组存储,使得容器可以保存n个元素

	//5.访问,一样(set,map), unordered_map 有下标,at访问, multimap没有
	g7["C++"] = 10; g7["JAVA"] = 100; 
	try { g7.at("C");}catch(...){ }
	
	//6.迭代器相关
	unordered_map<string, int>::iterator it01 = g7.begin();
	unordered_map<string, int>::const_iterator it02 = g7.cbegin();
	//没有反向迭代器, 增加了 桶迭代器local_iterator, const_local_iterator
	//表示第n个桶的首元素(begin(n),cbegin(n))和尾后迭代器(end(n),cend(n))
	unordered_map<string, int>::local_iterator it03 = g7.begin(0); 

	//7.插入,一样,insert,emplace, 包括下标插入
	//8.删除(erase) 都一样
	//9.特有算法一样,find,count,lower_bound, upper_bound, equal_range
	
	return 0;
}