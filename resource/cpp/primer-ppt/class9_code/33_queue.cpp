///queue
#include <iostream>
#include <queue>
#include <vector>
#include <string>
using namespace std;

struct A {
	A(const string& s,int d):s1(s),d1(d){}
	string s1; int d1;
};
int main() {
	//queue 队列(先进先出), 默认是 deque实现 【容器适配器】#include <queue>
	//template<class _Ty,class _Container=deque<_Ty>>class stack;
	queue<int> q1; //默认构造(用deque实现)
	//queue<int, vector<int>> q2; 不能vector,没有pop_front()
	//queue<int> q = { 1,2,3 }; 不行
	queue<int> q3(q1);
	queue<int> q4 = q1; //拷贝构造
	
	q4 = q1; //赋值,没有assign
	q4.swap(q1);
	std::swap(q4, q1); //可以高效swap
	cout << q4.size() << endl;  //当前元素个数
	cout << q4.empty() << endl; //是否为空,空返回true;
	
	//不支持迭代器,也没有 begin,end 等操作
	
	//插入元素：
	queue<A> q5;
	q5.push(A("abc", 1)); //同样有const T& 和T&&
	A a1("aaa", 2);
	q5.push(a1);
	q5.emplace("ccc", 4); //等同push,不过参数是A的构造参数

	//访问元素：只能访问队列头部和尾部元素
	q5.back() = { "bbb",3 }; //back()返回的是左值引用
	q5.front(); //队列头部元素

	//删除元素：只能删除栈顶元素
	if (!q5.empty()) q5.pop(); //pop()返回void

	return 0;
}