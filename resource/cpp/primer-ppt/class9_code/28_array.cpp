///array
#include <iostream>
#include <utility>
#include <string>
#include <array>
using namespace std;

int main() {
	//array(没有空间配置器) #include <array> (原生数组的一个封装)
	//明确知道最多要多少个元素，可以考虑array
    //直接 array<int,10> a; 元素是在栈上的。注意大小（栈空间有限）

	//template<class _Ty, size_t _Size> class array{ _Ty _Elems[_Size]; ...}
	//1、初始化
	array<int, 10> a1;  //默认构造,值没有初始化
	array<int, 3> a2{}; //值初始化,都是0
	array<string, 4> a3 = { "abc" }; //第1个按列表初始化,后面的默认初始化""
	array<int, 3> a4(a2);
	array<int, 3> a5 = a2;//拷贝(区别于内置数组,内置数组不能这样操作)
	array<string, 4> a6 = std::move(a3); //移动,每个元素都执行移动(或拷贝)
	//array<int, 3> a7(a2.begin(), a2.end()); 不支持迭代器范围初始化
	//array<int, 6> a8(6);
	//array<int, 6> a8(6,0); 指定几个元素的初始化不行
	
	//2.赋值
	a5 = a4; //内置数组不支持该操作
	a5 = { 2,3 }; //用初始化列表赋值
	//不支持 assign操作
	a5.fill(9); //所有元素都赋值为9
	
	//3.swap交换,2个array交换,O(n)时间复杂度(每个元素交换)
	array<int, 10> a10 = { 1,2,3 };
	array<int, 10> a11 = { 5,6,7,8 };
	a10.swap(a11);
	std::swap(a10, a11); //同上
	//4.容量相关
	a10.size(); //元素个数,同非类型模板参数
	a10.empty(); //除非 array<int,0> a10;否则不可能为true
	a10.max_size(); //同size()
	//下面的都不支持
	//a10.capacity(); //a10.reserve(100); //a10.resize(25);
	
	//5.元素访问, 与vector一样
	a10[2] = 10; //下标访问,要保证下标不越界,否则未定义结果
	a10.at(1) = 20;//下标越界抛out_of_range异常
	a10.front(); //第1个元素的引用
	a10.back(); //最后一个元素的引用

	//6.迭代器相关 , 尺寸要匹配,与vector一样
	array<int, 10>::iterator it1 = a10.begin();
	array<int, 10>::const_iterator it2 = a10.cbegin();
	array<int, 10>::reverse_iterator it3 = a10.rbegin();
	array<int, 10>::const_reverse_iterator it4 = a10.crbegin();
	
	//7.插入元素,不存在
	//8.删除元素,不存在 clear()也没有
	
	//9. == != < > <= >= 都有, 但是类型要一样
	array<int, 2> a12 = { 1,2 };
	array<int, 3> a13 = { 1,2,3 };
	//cout << (a12 == a13) << endl; 不行,类型不匹配
	
	//10.C接口 同vector
	array<char, 10> a16 = {'a','b','c','\0'};
	printf("%s\n", a16.data()); //abc
	//printf("%s\n", a16.begin()); //错
	printf("%s\n", &(*a16.begin())); //OK
	return 0;
}
