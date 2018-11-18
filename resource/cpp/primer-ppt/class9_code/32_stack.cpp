///stack
#include <iostream>
#include <stack>
#include <vector>
#include <string>
using namespace std;

struct A {
	A(const string& s, int d) :s1(s), d1(d) {}
	string s1; int d1;
};
int main() {
	//stack 栈(先进后出), 默认是 deque实现 【容器适配器】#include <stack>
	//template<class _Ty,class _Container=deque<_Ty>>class stack;
	stack<int> s1; //默认构造(用deque实现)
	stack<int, vector<int>> s2; //默认构造(用vector实现)
	//stack<int> s1 = { 1,2,3 }; 不行
	stack<int> s3(s1);
	stack<int> s4 = s1; //拷贝构造

	s4 = s1; //赋值,没有assign
	s4.swap(s1);
	std::swap(s4, s1); //可以高效swap
	cout << s4.size() << endl;  //当前元素个数
	cout << s4.empty() << endl; //是否为空,空返回true

	//不支持迭代器,也没有 begin,end 等操作

	//插入元素：
	stack<A> s5;
	s5.push(A("abc", 1)); //同样有const T& 和T&&
	A a1("aaa", 2);
	s5.push(a1);
	s5.emplace("ccc", 4); //等同push,不过参数是A的构造参数

	//访问元素：只能访问栈顶元素
	s5.top() = { "bbb",3 }; //top()返回的是左值引用
	cout << s5.top().s1 << s5.top().d1 << endl;

	//删除元素：只能删除栈顶元素
	if (!s5.empty()) s5.pop(); //pop()返回void

	return 0;
}
