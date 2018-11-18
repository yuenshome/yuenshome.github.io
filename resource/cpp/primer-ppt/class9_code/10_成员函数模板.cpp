#include <iostream>
using namespace std;

class Base{};
class D1:public Base{};

template<typename T>
class SmartPtr {
public:
	explicit SmartPtr(T* p=nullptr)
		:ptr(p), count(new int(p ? 1 : 0)) { }
	~SmartPtr() {
		if(--(*count)<=0){delete ptr;delete count;}
	}
	void swap(SmartPtr& rhs) {
		std::swap(ptr, rhs.ptr);
		std::swap(count, rhs.count);
	}
	//拷贝构造
	SmartPtr(const SmartPtr& rhs) 
		:ptr(rhs.ptr), count(&(++*rhs.count)) {}
	template<typename U> 
	SmartPtr(const SmartPtr<U>& rhs)
		:ptr(rhs.ptr), count(&(++*rhs.count)) {}
	//赋值
	SmartPtr& operator=(const SmartPtr& other);
	template<typename U>
	SmartPtr& operator=(const SmartPtr<U>& other);
private:
	T* ptr;
	int* count;
	//所有的SmartPtr<...>都是SmartPtr<...>的友元
	template<typename X> friend class SmartPtr;
	//friend class Test<T>; 
	//Test<int> 是 Smart<int>的友元,匹配参数
};

template<typename T>
template<typename U> //次序不能错
SmartPtr<T>& SmartPtr<T>::operator=
					(const SmartPtr<U>& other){
	SmartPtr(other).swap(*this);
	return *this;
}
template<typename T>
SmartPtr<T>& SmartPtr<T>::operator=
					(const SmartPtr& other) {
	SmartPtr(other).swap(*this);
	return *this;
}
int main() {
	//基类指针指向派生类对象
	Base *pb = new D1;
	delete pb;

	SmartPtr<Base> s_pb1 = SmartPtr<D1>(new D1);
	SmartPtr<Base> s_pb2;
	s_pb2 = s_pb1;
	s_pb2 = SmartPtr<D1>(new D1);
	return 0;
}