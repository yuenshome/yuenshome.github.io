///vector
#include <iostream>
#include <utility>
#include <string>
#include <vector>
using namespace std;
struct A {
	A(const string& ss1="",int i1=0):s1(ss1),d1(i1){}
	string s1;
	int d1;
};
int main() {
	//vector 动态数组 #include <vector>
	//尾部删除、添加，速度快。	遍历非常快。内存紧凑，使用的空间较少。

	//template<class _Ty, class _Alloc = allocator<_Ty>> class vector...
	//1、初始化：
	vector<int> v1; //默认构造
	vector<double> v2{ 1.0,2.0,3.0 };		 //初始化列表
	vector<string> v3 = { "abc","C++","C" }; //初始化列表
	vector<double> v4(v2);  
	vector<string> v5 = v3; //拷贝构造
	vector<string> v6 = std::move(v3); //移动构造
	vector<double> v7(v2.cbegin(), v2.cend()); //迭代器范围构造
	vector<int> v8(10); //10个元素,每个元素为0（值初始化）
	vector<int> v9(10, 1); //10个元素,每个元素为1
	//2.赋值 (会冲掉原先的内容)
	v8 = v1;
	v8 = { 1,2,3 }; //用初始化列表赋值
	v8.assign(v9.cbegin(), v9.cend()); //迭代器范围赋值
	v8.assign({ 1,2,3 }); //初始值列表赋值
	initializer_list<int> il = { 1,2,3 };
	v8.assign(il);
	v8.assign(10, 2);  //用10个2赋值
	//3.swap 交换,2个vector交换 O(1)时间复杂度
	vector<int> v10(20,1);  //20个元素都是1
	vector<int> v11(10, 2); //10个元素都是2
	v10.swap(v11);
	std::swap(v10, v11); //会执行上面的函数
	//4.容量相关
	v10.size(); //元素个数
	v10.empty(); //元素个数==0则返回true
	v10.max_size(); //能存储的最大元素个数
	v10.capacity(); //当前能存储的最大元素个数
	v10.reserve(100); //只能扩容,不能缩减,不改变元素个数
	cout << v10.size() << endl;
	v10.resize(25); //改变元素个数,删掉多余元素,或以值初始化添加元素
	v10.resize(30, 2); //改变元素个数,删掉多余元素,或以2添加元素
	v10.shrink_to_fit(); //降低容量,但是不是强制性的,只是通知stl可以降低容量
	//5.元素访问
	v11[1]=2; //下标访问,要保证下标不越界,否则未定义结果
	v11.at(1)=20;//下标越界抛out_of_range异常
	if(!v11.empty()) v11.front()=1; //第1个元素的引用
	if (!v11.empty()) v11.back()=2; //最后一个元素的引用
	//6.迭代器相关
	vector<int>::iterator it1 = v11.begin();
	vector<int>::const_iterator it2 = v11.cbegin();
	vector<int>::reverse_iterator it3 = v11.rbegin();
	vector<int>::const_reverse_iterator it4 = v11.crbegin();
	//end()也是一样
	//7.插入元素
	vector<A> v12;
	v12.push_back(A("123", 1)); //尾部添加元素
	A a1("abc", 2);
	v12.push_back(a1); //元素存入容器是拷贝 const A&, A&&
	v12.push_back(std::move(a1)); //push_back进右值,移动
	v12.emplace_back("abcd",3); //尾部添加元素,直接调用A的构造
	//在迭代器位置前插入以参数"aac",9构造的 A对象,返回指向新插入元素的迭代器
	v12.emplace(v12.begin(), "aac", 9); 
	A a2("acc", 3);
	vector<A>::iterator it_insert =  v12.insert(v12.begin(), a2); 
	//在迭代器位置前插入a2,返回指向新插入元素的迭代器
	cout << (*it_insert).s1 << "--" << (*it_insert).d1 << endl;
	//在迭代器位置前插入3个a2,返回指向第1个新元素的迭代器
	v12.insert(v12.begin(), 3, a2);
	vector<A> v13;
	//在迭代器位置前插入 v12.begin()到v12.end()之间的元素,返回指向第1个新元素的迭代器
	//若 v12.begin() == v12.end() 表示不插入一个元素,返回还是原迭代器位置
	v13.insert(v13.begin(), v12.begin(), v12.end());
	v13.insert(v13.begin(), { {"aa",1},{"bb",2} }); //用初始值列表插入
	//8.删除元素
	v13.pop_back(); //删除最后1个元素
	//删除迭代器位置的元素,返回指向被删元素后面元素的迭代器,若迭代器是end(),则行为未定义
	v13.erase(v13.begin()); 
	//删除迭代器范围,返回最后1个删除元素之后元素的迭代器
	v13.erase(v13.begin() + 1, v13.end()); 
	v13.clear(); //清空所有元素,但是容量不会变(capacity()不变)
	//9.关系运算
	//所有容器都支持 == != 运算符,除无序容器外都支持 < > <= >=
	vector<int> v14 = { 1,2,3 };
	vector<int> v15 = { 1,2,4 };
	v14 < v15; //true ,每个元素的比较
	v14 == v15; //false 
	//10.与C的接口
	vector<char> v16(10, '\0');
	strcpy(&v16[0], "abcd"); 
	printf("%s\n", &v16[0]); //abcd
	strcat(v16.data(), "1234");
	printf("%s\n", v16.data()); //abcd1234
	//printf("%s\n", v16.begin()); //错
	printf("%s\n", &(*v16.begin())); //OK
	//行为像指针,但是所指对象的地址是 &*迭代器
	cout << "----\n";
	return 0;
}