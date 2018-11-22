///map,multimap
#include <iostream>
#include <utility>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <functional>
#include <algorithm>
using namespace std;

int main() {
	//map/multimap: 一种 key-value 的数据结构
	//map(关键字不能重复), multimap(关键字可以重复) 头文件 #include <map> 红黑树
	//template<class K,class V,class cmp=less<K>,class _Alloc=allocator<pair<const K,V>>>
	//class map/multimap...  有4个模板参数  注意：实际存放的是 pair<const K,V>
	//map与set都是同一套体系，红黑树，接口封装有区别

	//1.初始化
	map<string, int, less<string>> m1; //默认是less<string>
	map<double, string, greater<double>> m2;
	map<string, int> m3{ {"C++",80},{"Python",90 },{"C",85} };
	map<string, int> m4 = m3; //拷贝构造
	map<string, int> m7(m3.begin(), m3.end()); //迭代器范围
	multimap<string, int> m8 = 
			{ {"C++",80},{"Python",90},{"C",85},{"C",83} };
	for (const auto& item : m8)
		cout << item.first << ":" << item.second << "  ";
	cout << endl;
	cout << "------------------\n";
	//2.赋值 (没有assign), swap 都与set相同
	m3 = m4; m4 = { { "C++",80 },{ "Python",90 } };
	m3.swap(m4); std::swap(m3, m4);
	//3.容量相关,与set一致
	m4.size(); m4.empty(); m4.max_size();
	
	//4.元素访问 
	//map有下标和at访问,multimap没有(因为key可能重复,不知道哪个key)
	//m7.at("D"); 因为没有找到,at会抛异常，所以不建议使用
	m7["C"] = 100; 
	cout << m7["C"] << " " << m7.at("C") << endl;
	//假如下标对应的key没有找到,则自动插入一个key,value默认初始化
	cout << m7["JAVA"] << endl; //"JAVA"没有,自动插入,value=0
	cout << "------------------\n";

	//5.迭代器相关,与set一致
	//6.特有算法,与set类似
	cout << m7.count("C++") << endl; //1
	cout << m8.count("C") << endl;   //2
	auto it1 = m7.find("C++");
	if (it1 != m7.end()) { //找到
		//注意: 迭代器解引用得到的是 一个 pair<const string,int>
		cout << (*it1).first << ":" << (*it1).second << endl;
		pair<const string, int> &pa1 = *it1; //注意const
	}
	//lower_bound, upper_bound, equal_range与set类似

	//7.插入 insert emplace
	//map可以通过下标直接插入,multimap不行
	//直接下标相当于: auto it=m7.insert(make_pair(string("PHP"),0)).first;
	//               return (*it).second; //将value以引用返回 
	m7["PHP"] = 99; //没有"PHP"直接插入,有则修改其对应的 value
	pair<map<string, int>::iterator, bool> res = m7.insert({ "GO",60 });
	cout << (*res.first).first << ":" << (*res.first).second << endl;
	cout << "是否成功？ "<<res.second << endl;
	m7.insert(pair<string, int>("GO", 60));
	m7.insert(make_pair("GO", 60));
	m8.emplace("CPP", 30);
	m8.insert(m7.begin(), m7.end());

	//8.删除 erase 与set一样
	m7.erase("C");
	m7.erase(m7.begin(), m7.end());

	return 0;
}