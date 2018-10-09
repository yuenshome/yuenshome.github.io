#include <iostream>
#include <memory>
using namespace std;
void fun(shared_ptr<int> ptr) { /*...*/ }
int main() {
	//1.一个裸指针不要用两个shared_ptr或unique_ptr来管理
	int *pi = new int(1); //少写这样的代码
	shared_ptr<int> pi1(pi);
	//shared_ptr<int> pi2(pi); //错
	shared_ptr<int> pi2 = pi1; //只能这样做

	//2.不要混用普通指针和智能指针
	shared_ptr<int> px1 = make_shared<int>(2);
	fun(px1); //ok,引用计数+1
	cout << *px1 << endl; //ok

	int *px = new int(2);
	//fun(px); //错,shared_ptr<int> ptr = px;不行
	fun(shared_ptr<int>(px)); //虽然可以运行，但是：
	cout << *px << endl; //此时 px已经被释放了！！

	//3.不要使用get初始化或者赋值给另一个智能指针
	shared_ptr<int> p(new int(3));
	int *p1 = p.get(); //获取p管理的裸指针,ok
	{
		//shared_ptr<int> p2(p1); //错，类似第1条
	}

	//4.不要delete get()返回的裸指针
	int *p2 = p.get();
	//delete p2; //错，会多次释放
	return 0;
}