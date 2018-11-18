#include <iostream>
#include <typeinfo>
using namespace std;

template <typename T,unsigned N=10>
class A {
public:
	//T* 起个别名, 注意类外使用A<T,N>::iterator的方式
	typedef T* iterator; 
	typedef const T* const_iterator;
	A(std::initializer_list<T> il) { 
		count++; 
		int i = 0;
		for (auto it = il.begin(); it != il.end(); ++it, i++)
			data[i] = *it;
	}
	~A() { count--; }
	static int get_count() { return count; }
	iterator begin() { return &data[0]; }
	iterator end() { return &data[N - 1]; }
public:
	static int count;
private:
	T data[N];
};

template <typename T,typename V>
typename T::iterator my_find(T &c, const V& value) {
	cout << typeid(c).name() << endl;
	typename T::const_iterator * c_it; //类的类型成员
	T::count * 3; //static成员
	typename T::iterator it = c.begin();
	while (it != c.end() && *it != value)
		++it;
	return it;
}

template<typename T>
class B {
public:
	typename T::iterator first(T& t) {
		return t.begin();
	}
};

template<typename T,unsigned N>
int A<T,N>::count = 0;
int main() {
	A<int> a1 = { 1,2,3,4,5 };
	auto it = my_find(a1, 3);
	if(it!=a1.end())
		cout << *it << endl; //3

	B< A<int> > b1;
	cout << *(b1.first(a1)) <<endl;//1
	return 0;
}