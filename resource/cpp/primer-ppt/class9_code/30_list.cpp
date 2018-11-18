///list
#include <iostream>
#include <utility>
#include <string>
#include <list>
using namespace std;
struct A {
	A(const string& ss1 = "", int i1 = 0) :s1(ss1), d1(i1) {}
	string s1;
	int d1;
};
int main() {
	//list 双向链表 #include <list>
	//不支持随机访问元素，访问头部和尾部元素很快。任何位置插入删除元素都很快，O(1)
	//插入和删除不会造成迭代器失效。空间成本较高（有2个指针）
	//template<class _Ty, class _Alloc = allocator<_Ty>> class list ...
	//1.初始化 与vector一样
	list<int> l1; //默认构造
	list<int> l2{ 1,2,3 };
	list<double> l3 = { 1.0,2.0,3.0 }; //初始化列表
	list<int> l4(l2);
	list<int> l5 = l2; //拷贝构造
	list<double> l6(l3.cbegin(), l3.cend()); //迭代器范围
	list<string> l7(10);
	list<string> l8(10, "abc"); //10个元素，每个都是"abc"

	//2.赋值  与vector一样
	l1 = l2; //类型要一致
	l2 = { 2,3,4 }; //初始值列表赋值
	l2.assign({ 1,2,3 });
	l2.assign(10, 0);
	l2.assign(l1.cbegin(), l1.cend());

	//3.swap 与vector一样
	//4.容量相关 不需要预先准备空间
	l2.size(); //元素个数
	l2.empty(); //元素个数==0则返回true
	l2.max_size(); //能存储的最大元素个数
	//l2.capacity(); list没有capacity(不存在扩容这样的操作)
	//l2.reserve(100); 没有
	l2.resize(25); //改变元素个数,删掉多余元素,或以值初始化添加元素
	l2.resize(30, 2); //改变元素个数,删掉多余元素,或以2添加元素
	//l2.shrink_to_fit(); 不需要预先准备空间,有1个元素开辟1个空间

	//5.元素访问 不能随机访问,头尾快,中间慢
	//l2[1] = 2; list 不行
	//12.at(1) = 20; list 不行
	if (!l2.empty()) l2.front() = 1; //第1个元素的引用
	if (!l2.empty()) l2.back() = 2; //最后一个元素的引用
	//需要访问第5个元素：(list访问中间的元素要遍历)
	list<int> l10 = { 1,2,3,4,5,6,7 };
	auto it = l10.begin();
	for (int i = 0; i < 4; ++i) ++it;
	cout << *it << endl;
	it = l10.begin();
	std::advance(it, 4); //it迭代器像前移动4步
	cout << *it << endl;
	it = l10.begin();
	auto it0=std::next(it,4);//从it向前移4步,但it不变,返回值是移后的位置
	cout << *it0 << "  " << *it << endl;

    //6.迭代器相关 与vector一样,因为是双向链表,所有反向迭代器ok
	//7.插入元素 支持 push_front(类似deque) 其他与vector一样
	//push_back,emplace_back,emplace,insert
	l10.push_front(100);

	//8.删除元素 支持pop_back pop_front
	if(!l10.empty()) l10.pop_back(); //删除最后1个元素
	if (!l10.empty()) l10.pop_front();//删除第1个元素
	//删除迭代器位置的元素,返回指向被删元素后面元素的迭代器,若迭代器是end(),则行为未定义
	l10.erase(l10.begin());
	//删除迭代器范围,返回最后1个删除元素之后元素的迭代器
	l10.erase(l10.begin() /*+1不支持*/, l10.end());
	l10.clear(); //清空所有元素

	//9.关系运算, 类似vector == != < > <= >=
	return 0;
}
