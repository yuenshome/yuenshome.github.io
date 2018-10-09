#include <iostream>
#include <memory>
#include <cassert>
using namespace std;
class T {
public:
	T(int i = 0) :data(i) { cout << "T构造\n"; }
	~T() { cout << "T析构\n"; }
	void show()const { cout << data << endl; }
private:
	int data;
};
class Unique_ptr {
public:
	Unique_ptr():ptr(nullptr) {	}
	explicit Unique_ptr(T* p):ptr(p){ }
	Unique_ptr(const Unique_ptr&) = delete;
	Unique_ptr& operator=(const Unique_ptr&) = delete;
	Unique_ptr(Unique_ptr&& other) noexcept//移动构造
		:ptr(other.ptr) { 
		other.ptr = nullptr;
	}
	Unique_ptr& operator=(Unique_ptr&& other) noexcept {
		if (this == &other) return *this; //移动赋值
		del();
		std::swap(ptr, other.ptr);
		return *this;
	}
	~Unique_ptr() { //析构
		del();
	}
public:
	T& operator*() {
		assert(ptr);
		return *ptr;
	}
	T* operator->() {
		assert(ptr);
		return ptr;
	}
	operator bool()const {
		return nullptr != ptr;
	}
	T* release() { //交出控制权
		T* p = ptr;
		ptr = nullptr;
		return p;
	}
	void reset(T* new_p=nullptr) {//重置
		del();
		ptr = new_p;
	}
	T* get() { return ptr; } //返回裸指针
private:
	void del() { delete ptr; ptr = nullptr; }
private:
	T* ptr; //管理的资源
};

int main() {
	Unique_ptr p1; //空指针
	//Unique_ptr p2 = new T(100); //错误
	Unique_ptr p2(new T(100));
	(*p2).show(); //100 行为像指针 *
	p2->show();   //100 行为像指针 ->
	if (p2) cout << "p2 not null!\n";

	//可以移动拷贝
	Unique_ptr p3 = std::move(p2);
	if (!p2) cout << "p2 empty\n";
	//可以移动赋值
	p1 = std::move(p3);

	T* p = p1.get(); //获取裸指针
	p->show(); //100
	p = p1.release();
	p->show(); //100 注意，p是裸指针,要delete才行

	Unique_ptr p4(p); //重新接管
	p4.reset(); //释放p4管理的资源
	cout << "----------\n";

	Unique_ptr p5(new T(200));
	//先释放管理的资源，再接管新资源
	p5.reset(new T(300));
	cout << "==========\n";
	return 0;
}