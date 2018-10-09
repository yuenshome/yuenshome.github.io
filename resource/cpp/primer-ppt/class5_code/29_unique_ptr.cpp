#include <iostream>
#include <memory>
#include <functional>
using namespace std;
class T {
public:
	T(int i = 0) :data(i) { cout << "T构造\n"; }
	~T() { cout << "T析构\n"; }
	void show()const { cout << data << endl; }
private:
	int data;
};
//值传递：拷贝
void print1(unique_ptr<T> p) {
	p->show();
}
//引用传递: 
void print2(const unique_ptr<T> &p) {
	p->show();
}
void print(const T& obj) {
	cout << "对象的引用\n";
	obj.show();
}
void del_T_arr(T* parr) { //删除器
	delete[] parr;
}
//shared_ptr 用法(接口)：
int main() {
	unique_ptr<T> p1; //空指针
	//explicit构造,只能直接初始化
	//unique_ptr<T> p2 = new T(100); //错误
	unique_ptr<T> p2(new T(100));
	//没有use_count(),unique(),因为没有引用计数
	(*p2).show(); //100 行为像指针 *
	p2->show();   //100 行为像指针 ->
	if (p2) cout << "p2 not null!\n";

	//没有普通拷贝构造函数
	//unique_ptr<T> p3(p2); //错误
	//unique_ptr<T> p3 = p2;//错误
	//没有普通赋值
	//p1 = p2; //错误

	//可以移动拷贝
	unique_ptr<T> p3 = std::move(p2);
	if (!p2) cout << "p2 empty\n";
	//可以移动赋值
	p1 = std::move(p3);

	T* p = p1.get(); //获取裸指针
	p->show(); //100
	
	//release(),放弃控制权,返回指针
	//p1.release(); //错,release不会释放资源
	p = p1.release();
	p->show(); //注意，p是裸指针,要delete才行

	unique_ptr<T> p4(p); //重新接管

	p4.reset(); //释放p4管理的资源
	cout << "----------\n";

	unique_ptr<T> p5(new T(200));
	//先释放管理的资源，再接管新资源
	p5.reset(new T(300));
	cout << "==========\n";

	//智能指针作为参数传递
	print1(std::move(p5)); //值传递
	unique_ptr<T> p6(new T(400));
	print2(p6); //引用传递
	print(*p6);
	cout << "----------------\n";

	//删除器：
	unique_ptr<T, function<void(T*)>> p11(new T[3], del_T_arr);
	unique_ptr<T, function<void(T*)>> p12(new T[3]{ 7,8,9 }, [](T* p)->void {delete[] p; });
	cout << "===============\n";
	return 0;
}