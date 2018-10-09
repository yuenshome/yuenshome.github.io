///shared_ptr用法
#include <iostream>
#include <memory>
using namespace std;
class T {
public:
	T(int i = 0) :data(i) { cout << "T构造\n"; }
	~T() { cout << "T析构\n"; }
	void show()const { cout << data << endl; }
private:
	int data;
};
//值传递：拷贝，引用计数+1
void print1(shared_ptr<T> p) {  
	cout <<"值: "<< p.use_count() << endl;
	p->show();
}
//引用传递: 引用计数不会+1
void print2(const shared_ptr<T> &p) { 
	cout << "引用: "<<p.use_count() << endl;
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
	shared_ptr<T> p1; //空指针
	//use_count() 引用计数(同时有几个指针指向对象)
	cout << p1.use_count() << endl; //0
	//unique(),use_count()==1返回true,否则false
	cout << p1.unique() << endl; //0

	//shared_ptr<T> p2= new T(1);//不行,explicit构造
	shared_ptr<T> p2(new T(10));
	cout << p2.use_count() << endl; //1
	cout << p2.unique() << endl; //1
	(*p2).show(); //10 行为像指针 *
	p2->show();   //10 行为像指针 ->
	if (p2) cout << "p2 not null!\n";

	shared_ptr<T> p3 = p2; //拷贝构造
	shared_ptr<T> p4(p2);  //拷贝构造
	cout << p2.use_count() << endl; //3
	cout << p4.use_count() << endl; //3
	cout << p3.unique() << endl; //0

	//推荐用这样的方式初始化
	shared_ptr<T> p5 = make_shared<T>(20);
	//返回智能指针中保存的裸指针，谨慎使用！
	T* p = p5.get();
	p->show(); //20

	shared_ptr<T> p6;
	p6 = p5; //赋值

	//reset() 无参数时,不再管理资源,意味着count--,可能会释放资源
	p3.reset(); 
	cout << p2.use_count() << endl; //2
	//reset(T* p),不再管理原资源,接管新的资源(裸指针)
	cout << p4.use_count() << endl; //2
	p4.reset(new T(30));
	cout << p4.use_count() << endl; //1

	//swap() 交换资源
	p4->show(); //30
	p4.swap(p5); 
	p4->show(); //20
	std::swap(p4, p5); //效果与 p4.swap(p5)一样

	//智能指针作为参数传递
	print1(p4); //值传递
	print2(p4); //引用传递
	print(*p4);
	cout << "--------------------\n";
	//删除器：
	shared_ptr<T> p11(new T[3], del_T_arr);
	shared_ptr<T> p12(new T[3]{7,8,9}, [](T* p) {delete[] p; });
	T* pa = p12.get();
	pa = pa + 2;
	pa->show();
	cout << "--------------------\n";
	return 0;
}