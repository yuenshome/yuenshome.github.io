///deque
#include <iostream>
#include <utility>
#include <string>
#include <deque>
using namespace std;
struct A {
	A(const string& ss1 = "", int i1 = 0) :s1(ss1), d1(i1) {}
	string s1;
	int d1;
};
int main() {
	//deque 双端队列 #include <deque>
	//随机访问元素，头部和尾部添加删除元素效率很高，中间删除和添加效率低。
	//元素的访问和迭代比vector要慢，迭代器不能是普通的指针。
	//template<class _Ty, class _Alloc = allocator<_Ty>> class deque ...	
	//1.初始化,与vector一样
	deque<int> d1; //默认构造
	deque<int> d2{ 1,2,3 };
	deque<double> d3 = { 1.0,2.0,3.0 }; //初始化列表
	deque<int> d4(d2);
	deque<int> d5 = d2; //拷贝构造
	deque<double> d6(d2.cbegin(), d2.cend()); //迭代器范围,元素可转换即可
	deque<string> d7(10); //10个元素，每个都是空串
	deque<string> d8(8, "abc"); //8个元素，每个都是"abc"

	//2.赋值,与vector一样
	d8 = d7;
	d8 = { "aaa","bbb","ccc" }; //用初始化列表赋值
	d8.assign(d7.cbegin(), d7.cend()); //迭代器范围赋值
	d8.assign({ "aa","bb","cc" }); //初始值列表赋值
	initializer_list<string> il = { "a","b","c" };
	d8.assign(il);
	d8.assign(10, "abc");  //用10个"abc"赋值

	//3.swap 与vector一样,O(1)
	deque<int> d10, d11{ 1,2,3 };
	d10.swap(d11);	std::swap(d10, d11);

	//4.容量相关 没有capacity和reserve,其他与vector一样
	d10.size(); //元素个数
	d10.empty(); //元素个数==0则返回true
	d10.max_size(); //能存储的最大元素个数
	//d10.capacity(); deque没有
	//d10.reserve(100); deque没有
	cout << d10.size() << endl;
	d10.resize(25); //改变元素个数,删掉多余元素,或以值初始化添加元素
	d10.resize(30, 2); //改变元素个数,删掉多余元素,或以2添加元素
	d10.shrink_to_fit(); //降低容量,但是不是强制性的,只是通知stl可以降低容量
	
	//5.元素访问 与vector一样 [] at front back
	//6.迭代器相关 与vector array 一样
	
	//7.插入元素  多了push_front,emplace_front 其他与vector一样
	//push_back,emplace_back,emplace,insert
	deque<A> d12;
	d12.push_front(A("123", 1)); //头部添加元素
	if (!d12.empty()) d12.pop_front(); //头部删除元素
	d12.emplace_front("abc", 2);

	//8.删除元素 多了pop_front 其他和vector一样
	d12.pop_front(); //删除第1个元素
	d12.clear(); //清空所有元素,有可能释放空间(一般释放空间)
	//9.关系运算 和vector一样
	return 0;
}
