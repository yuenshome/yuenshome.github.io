#include <iostream>
using namespace std;
class A {
public:
	A(int i = 0) :pi(new int(i)) { cout << "A构造\n";}
	~A() {cout << "A析构！\n"; delete pi; }
	void show()const { cout << *pi << endl; }
private:
	int* pi;
};
char g_buf[200];
int main() {
	void* pbuf = malloc(100);
	//定位new,在 pbuf地址处：初始化一个对象
	//1.内存中 堆空间
	A *pa1 = new(pbuf) A(1);
	pa1->show();  //对象正确构造 1
	cout << pbuf << endl;
	cout << pa1 << endl;
	pa1->~A(); //显式调用析构
	free(pbuf);
	cout << "-------------------\n";
	char buf[200];
	//2.内存在 栈空间
	A* pa2 = new(buf) A(2);
	pa2->show(); //对象正确构造 2
	cout << (void*)buf << endl;
	cout << pa2 << endl;
	// delete pa2; //错误，定位new出来的对象不能delete
	pa2->~A(); //显式调用析构
	cout << "-------------------\n";
	//3.内存中data区
	A* pa3 = new(g_buf) A(3);
	pa3->show(); //对象正确构造 3
	cout << (void*)g_buf << endl;
	cout << pa3 << endl;
	pa3->~A();
	return 0;
}