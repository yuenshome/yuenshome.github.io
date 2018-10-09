#include <iostream>
#include <string>
#include <vector>
#include <memory>
using namespace std;

int main() {
	//定义一个allocator对象，为类型是string的对象分配内存
	allocator<string> alloc;
	//分配一段原始的、未构造的内存，可以保存10个string对象
	string * const  p = alloc.allocate(10);
	string* q = p;
	alloc.construct(q++); //在q的位置构造对象(默认构造)
	alloc.construct(q++, 5, 'A'); // "AAAAA"
	alloc.construct(q++, "ABC");  // "ABC"
	//执行对象的析构函数
	while (q != p)
		alloc.destroy(--q);
	//释放一段内存，p是前面allocte()返回的指针,内存段中的对象
	//必须已经析构（已经调用过destroy()）
	alloc.deallocate(p, 10);

	vector<string> vec = {"abc","xyz","c++","python"};
	string * const p1 = alloc.allocate(vec.size()+3);
	//通过拷贝vec中的元素来构造从 p1 开始的元素
	string * q1 = uninitialized_copy(vec.begin(), vec.end(), p1);
	//剩余元素初始化为 "hello"
	q1 = uninitialized_fill_n(q1, 3, "hello");
	string *q2 = p1;
	while (q2 != q1)
		cout << *q2++<<"\t";
	cout << endl;
	return 0;
}