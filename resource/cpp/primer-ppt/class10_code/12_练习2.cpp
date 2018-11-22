//实现 my_for_each,传入参数：容器,对容器执行操作
//for_each(beg,end,unaryOp) ==> my_for_each(容器的引用,unaryOp)
//自己写一个my_for_each
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

//找出下面代码的问题
template<typename C, typename F>
void my_for_each_1(C &c, F f) {
	for (auto it = c.begin(); it < c.end(); it++)
		f(*it);
}
//问题1: ++it (it++返回临时对象,效率低)
//问题2: it != c.end() (it < c.end() 有些容器迭代器不支持)
//问题3: for (auto it=c.begin(),it_end=c.end(); it!=it_end;it++) 效率会高一些
//问题4: 假如传入的是内置数组,所以: c.begin和c.end改写为std::begin(c)和std::end(c)
//问题5: 最简单写法  for_each(std::begin(c), std::end(c), f);
int main() {
	vector<int> vec = { 1,2,3 };
	for_each(vec.begin(), vec.end(), [](int& v) {v += 1; });
	for_each(vec.begin(), vec.end(), [](int& v) {cout << v << " "; });
	cout << endl;

	vec = { 1,2,3 };
	my_for_each_1(vec, [](int& v) {v += 1; });
	my_for_each_1(vec, [](int& v) {cout << v << " "; });
	cout << endl;

	return 0;
}