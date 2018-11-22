///set,multiset
///
#include <iostream>
#include <utility>
#include <string>
#include <vector>
#include <set>
#include <functional>
#include <algorithm>
using namespace std;

struct A {
	A(const string& ss1 = "", int i1 = 0) :s1(ss1), d1(i1) {}
	bool operator<(const A& rhs)const { //实现了 < 没有实现 ==
		if (s1 < rhs.s1) return true;
		if (!(rhs.s1 < s1) && d1 < rhs.d1) return true;
		return false;
	}
	string s1;
	int d1;
};

struct B {
	B(const string& ss1 = "", int i1 = 0) :s1(ss1), d1(i1) {}
	string s1;
	int d1;
};

class compB { //仿函数,> 返回true,所以set从大到小
public:
	bool operator()(const B& lhs, const B& rhs)const {
		if (lhs.s1 > rhs.s1) return true;
		if (!(rhs.s1 > lhs.s1) && lhs.d1 > rhs.d1) return true;
		return false;
	}
};

int main() {
	//关联容器的元素是按关键字来保存和访问的。
	//set(关键字不能重复), multiset(关键字可以重复) 头文件 #include <set> 红黑树
	//template<class K,class Cmp=less<K>,class _Alloc=allocator<K>> class set...
	//template<class K,class Cmp=less<K>,class _Alloc=allocator<K>> class multiset..

	//1.元素自动排序
	//2.插入和删除时间复杂度: O(logN)  [需求：频繁插入、删除、查找，并且要求有序]
	//3.元素必须支持“严格弱序”
	//  （1） (a < b)为真 ==> (b < a)为假, (a < a)肯定为假
	//  （2） (a < b) 并且 (b < c) ==> (a < c)
	//  （3） (a < b)为假 并且 (b < a)为假 ==> (a == b)
	//  （4） (a == b) 并且 (b == c) ==> (a == c)
	//4.在set中的元素，不能改变其值
	//5.以Node(结点)的方式存放,
	//内存消耗较大(左子树指针,右子树指针,父结点指针,前驱指针,后驱指针)

	//pair的结构: 
	//template<typename T1, typename T2> struct pair {
	//	T1 first;
	//	T2 second;
	//	//...
	//};
	pair<int, double> p1(1, 2.0);
	pair<int, char> p2 = { 10, 'c' };
	pair<int, double> p3 = make_pair(1, 3.0);

	//1.初始化
	set<int,less<int>> g1;  //默认就是 less<int>
	multiset<int,greater<int>> g2;
	set<double> g3{ 1.0, 3.0, 2.0, 2.0 }; //2.0只会存1次
	set<double> g4{ 1.0, 3.0 }; //初始值列表
	set<double> g5(g3);  
	set<double> g6 = g3; //拷贝构造
	vector<int> vec = { 1,2,3,2,4,4,6 };
	set<int> g7(vec.cbegin(), vec.cend());		 //1 2 3 4 6
	multiset<int> g8(vec.crbegin(), vec.crend());//1 2 2 3 4 4 6
	for (auto& item : g7) cout << item << " "; cout << endl;
	for (auto& item : g8) cout << item << " "; cout << endl;
	cout << "------------------\n";

	//2.赋值 (没有assign), swap
	g5 = g6; 
	g7 = { 1,2,3,4,6 }; //初始值列表赋值
	initializer_list<int> il = { 1,2,3,2,4,4,6 };
	g8 = il;
	g5.swap(g6);  std::swap(g5, g6); //交换高效O(1)

	//3.容量相关
	g7.size(); //元素个数
	g7.empty(); //元素个数==0则返回true
	g7.max_size(); //能存储的最大元素个数
	//以结点存放:没有capacity(),reserve(100),resize(25)

	//4.元素访问: 不支持下标访问,at访问, 没有front,back这样的操作
	//可通过迭代器遍历，或者find查找元素

	//5.迭代器相关
	set<int>::iterator it1 = g7.begin(); //*it1不能赋值
	set<int>::const_iterator it2 = g7.cbegin();
	set<int>::reverse_iterator it3 = g7.rbegin();//*it3不能赋值
	set<int>::const_reverse_iterator it4 = g7.crbegin();
	//end()也是一样  实际上begin()返回的迭代器也是const的

	//6.特有算法: count,find,lower_bound,upper_bound,equal_range
	//count: 标准算法的count是遍历的O(N)的,而set的是O(logN)
	cout<<g7.count(1)<<" "<<g7.count(10)<<endl;//set只返回1或0
	cout<<g8.count(2)<<endl; //multiset 返回>=0(有可能有多个)
	//find: 标准算法的find是遍历
	auto it5 = g7.find(1); //返回的是迭代器
	if (it5 != g7.end()) //需要和end()比较才能知道是否有该元素
		cout << "g7: 1找到了\n";
	else
		cout << "g7: 1没有找到\n";
	//lower_bound,upper_bound,equal_range,标准算法最好是随机访问迭代器
	auto it6 = g7.lower_bound(5); //返回第1个>=的元素位置
	if (it6 != g7.end()) //g7: 1 2 3 4 6
		cout << "lower_bound 5: " << *it6 << endl; //6的位置
	it6 = g7.upper_bound(5); //返回第1个>元素的位置
	if (it6 != g7.end())
		cout << "upper_bound 5: " << *it6 << endl;//6的位置
	//在multiset上用equal_range
	pair<set<int>::iterator,set<int>::iterator> range_1=g8.equal_range(2); 
	auto range_2 = g8.equal_range(2); //用auto更加方便
	//返回上面2个位置组成的pair 
	//return make_pair(g8.lower_bound(2),g8.upper_bound(2));
	//实际上：假如返回pair的first和second一致，说明没有找到该元素
	//g8: 1 2 2 3 4 4 6
	if (range_1.first!=g8.end()) cout<<"first: "<<*range_1.first<<" -- ";
	else cout << "first: end() -- ";
	if (range_1.second!=g8.end()) cout<<"second: "<<*range_1.second<<endl;
	else cout << "second: end()\n";
	cout << "------------------\n";

	//7.插入元素,没有push_back,emplace_back,push_front,emplace_front这样的操作
	//只有insert 和 emplace 操作,可插入单个元素,也可插入迭代器一个范围的元素
	g7.insert(10); //插入单个元素(返回值见后面)
	g7.emplace(11); //传入的是元素类型的构造参数(返回值同上)
	g7.insert(vec.begin(), vec.end()); //插入一个迭代器范围，返回void
	g7.insert({ 1,3,9 }); //插入一个初始值列表, 返回void
	//返回值: 对于 set: 返回一个pair,first:插入元素的位置,second:true还是false
	//对应插入set中已经存在的元素,返回pair的 second为false,插入操作不做任何事情
	for (auto& item : g7) cout << item << " "; cout << endl;
	auto r_pair = g7.insert(9); //9已经存在
	cout << "插入元素:"<<*r_pair.first <<" 是否成功:"<<r_pair.second << endl;
	r_pair = g7.insert(8); //8不存在，会插入
	cout << "插入元素:" << *r_pair.first <<" 是否成功:"<<r_pair.second<<endl;
	for (auto& item : g7) cout << item << " "; cout << endl;
	//返回值: 对于multiset,不存在插入失败的概念！所以返回插入元素的迭代器位置
	auto it9 = g8.insert(2);
	cout << "multiset插入不会失败: "<<*it9 << endl;
	cout << "------------------\n";

	//8.删除元素 没有pop_back,pop_front这样的操作
	cout <<g7.erase(1)<<endl; // 按元素的值来删除,返回删除元素的个数
	for (auto& item : g8) cout << item << " "; cout << endl;
	cout <<g8.erase(2)<<endl; //multiset 会把元素值是 2 的都删除了
	for (auto& item : g8) cout << item << " "; cout << endl;
	//erase(迭代器位置),返回删除位置后面的元素位置
	auto it11 = g8.find(3);
	if (it11 != g8.end())
		g8.erase(it11); //先找到3个位置,再删除。
	//erase(b,e) 删除迭代器范围,返回e
	g8.erase(g8.begin(), g8.end());
	g7.clear(); //清空
	//9.比较运算 == != < <= > >= 都有

	cout << "------------------\n";
	//观察: set的find 是通过 < 运算符来查找的, 标准算法find是通过==
	set<A> a1 = { { "abc",1 },{ "eee",6 },{ "eee",3 },{ "kkk",10 } };
	for (auto& item : a1) cout << item.s1 << ":" << item.d1 << "  ";
	cout << endl;
	auto it = a1.find(A("eee", 3));
	if (it != a1.end()) {
		cout << "A(\"eee\", 3) find! "<<(*it).s1<<":"<<(*it).d1<<endl;
	}
	//std::find(a1.begin(), a1.end(), A("eee", 3)); 
	//出错：提示A类型没有operator==


	set<B, compB> b1= { { "abc",1 },{ "eee",6 },{ "eee",3 },{ "kkk",10 } };
	for (auto& item : b1) cout << item.s1 << ":" << item.d1 << "  ";
	cout << endl;
	auto it0 = b1.find(B("eee", 3));
	if (it0 != b1.end()) {
		cout << "A(\"eee\", 3) find! " << (*it0).s1 << ":" << (*it0).d1 << endl;
	}

	return 0;
}