///priority_queue
#include <iostream>
#include <queue>
#include <vector>
#include <string>
using namespace std;

struct A {
	A(const string& s, int d) :s1(s), d1(d) {}
	string s1; int d1;
};
struct compare_A {
	bool operator()(const A& lhs, const A& rhs)const {
		//先比较 s1,再比较 d1
		if (lhs.s1 < rhs.s1) return true;
		if (!(rhs.s1 < lhs.s1) && lhs.d1 < rhs.d1) return true;
		return false;
	}
};
int main() {
	//priority_queue 优先级队列(符合优先条件的先出), 
	//默认是vector实现 【容器适配器】#include <queue>
	//template<class _Ty, class _Container = vector<_Ty>,
	//	class _Pr = less<typename _Container::value_type> >
	//	class priority_queue;

	//默认构造(用vector实现,优先级的比较是<,大顶堆,大数优先出队)
	//使用默认的 less<T> 就是元素要实现 < 运算符
	priority_queue<int> q1; 
	priority_queue<int> q2 = q1; //拷贝构造
	q1.swap(q2);
	std::swap(q1, q2); //高效交换
	cout << q1.size() << endl;
	cout << q1.empty() << endl;
	
	//不支持迭代器,也没有 begin,end 等操作

	//使用自定义的比较函数,要把三个模板参数都写出来
	priority_queue<A, vector<A>,compare_A> q3;

	//插入
	q3.push(A("123",4));
	q3.push(A("123",2));
	q3.push(A("abc",13));
	q3.push(A("abc", 5)); //传元素类型对象
	q3.emplace("abb", 2); //传构造参数
	
	//访问元素,只有top(),没有front, back
	cout << q3.top().s1 << "--" << q3.top().d1 << endl;

	//删除元素:
	if(!q3.empty()) q3.pop();
	
	return 0;
}