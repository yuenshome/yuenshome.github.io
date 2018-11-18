///forward_list
#include <iostream>
#include <utility>
#include <string>
#include <forward_list>
using namespace std;

int main() {
	//forward_list 单向链表 #include <forward_list>
	//不支持随机访问元素，访问头部元素很快。任何位置插入删除元素都很快，O(1)
	//插入和删除不会造成迭代器失效。空间成本较高（有1个指针）,比list节省
	//与手写的C风格的单链表相比,没有任何时间和空间上的额外开销(比如没有size)
	//template<class _Ty,class _Alloc = allocator<_Ty>> class forward_list
	
	//1.初始化,和vector list一样
	forward_list<int> l1; //默认构造
	forward_list<int> l2{ 1,2,3 };
	forward_list<double> l3 = { 1.0,2.0,3.0 }; //列表初始化
	forward_list<int> l4(l2);
	forward_list<int> l5 = l2; //拷贝构造
	//迭代器范围初始化,可以用其他容器,只要元素类型能转换(其他容器一样)
	forward_list<double> l6(l2.cbegin(), l2.cend());
	forward_list<string> l7(9);
	forward_list<string> l8(9, "aaa"); //9个元素,"aaa"

	//2.赋值  与vector list一样
	l1 = l2; //类型要一致
	l2 = { 2,3,4 }; //初始值列表赋值
	l2.assign({ 1,2,3 });
	l2.assign(10, 0);
	l2.assign(l1.cbegin(), l1.cend());

	//3.swap 与vector一样
	//4.容量相关 不需要预先准备空间
	//l2.size(); forward_list 没有元素个数size()函数
	l2.empty(); //元素个数==0则返回true
	l2.max_size(); //能存储的最大元素个数
	//l2.capacity(); 同list 没有
	//l2.reserve(100); 同list 没有
	l2.resize(25); //改变元素个数,删掉多余元素,或以值初始化添加元素
	l2.resize(30, 2); //改变元素个数,删掉多余元素,或以2添加元素
	//l2.shrink_to_fit(); 不需要预先准备空间,有1个元素开辟1个空间

	//5.元素访问 不能随机访问,头快,尾部和中间慢
	//l2[1] = 2; list 不行
	//12.at(1) = 20; list 不行
	if (!l2.empty()) l2.front() = 10; //第1个元素的引用
	//l2.back(); forward_list没有
	
    //6.迭代器相关 与vector一样,因为是双向链表,所有反向迭代器ok
	forward_list<int>::iterator it1 = l2.begin();
	forward_list<int>::const_iterator it2 = l2.cbegin();
	//forward_list<int>::reverse_iterator 没有 rbegin() 没有
	//forward_list<int>::const_reverse_iterator没有 crbegin()没有
	forward_list<int>::iterator it3 = l2.before_begin(); //多的
	forward_list<int>::const_iterator it4 = l2.cbefore_begin();//多的

	//7.插入元素 与list有区别
	//l2.push_back(100); 不支持
	l2.push_front(100); //头部插入,ok
	l2.emplace_front(101); //头部插入
	//emplace没有了，改用emplace_after
	l2.emplace_after(l2.before_begin(), 102); //在befor_begin()后插入102
	//insert没有了，改用insert_after, 在迭代器位置后面插入
	//返回指向最后1个插入元素的迭代器 (其他迭代器：返回第1个新元素的迭代器)
	l2.insert_after(std::next(l2.before_begin(), 2), 103);
	l2.insert_after(l2.before_begin(), l1.cbegin(), l1.cend());

	//8.删除元素 与list有区别
	if(!l2.empty()) l2.pop_front(); //删除第1个元素
	//l2.pop_back(); 没有
	//删除迭代器位置后面的元素,返回void,(其他容器:指向被删元素后面元素的迭代器)
	l2.erase_after(l2.before_begin()); //删第1个元素
	//删除迭代器范围,返回void,(其他容器:返回最后1个删除元素之后元素的迭代器)
	for (const auto& item : l2) cout << item << " "; cout << endl;
	l2.erase_after(l2.cbegin(), l2.end()); //第1个元素没有删掉
	for (const auto& item : l2) cout << item << " "; cout << endl;
	l2.erase_after(l2.cbefore_begin(), l2.end()); //全部删掉了
	for (const auto& item : l2) cout << item << " "; cout << endl;
	l2.clear(); //清空所有元素

	//9.关系运算, 类似list == != < > <= >=
	return 0;
}
