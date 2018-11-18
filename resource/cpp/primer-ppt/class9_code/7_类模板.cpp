#include <iostream>
#include <cassert>
using namespace std;

template <typename T,int MAXSIZE>
class MyStack{
public:
	MyStack():size(0){}
	MyStack(const MyStack& other):size(other.size){ //类内实现
		for (int i = 0; i < size; i++)
			data[i] = other.data[i];
	}
	MyStack& operator=(const MyStack& other);
	bool empty()const { return size == 0; }//类内实现,隐式内联
	bool full()const { return size == MAXSIZE; }
	void push(const T& elem) { 
		assert(!full()); data[size++] = elem;
	}
	void pop() { assert(!empty()); size--; }
	inline T& top();
private:
	T data[MAXSIZE]; //栈数组
	int size;		 //元素个数
};
template<typename T,int MAXSIZE>
MyStack<T, MAXSIZE>& MyStack<T, MAXSIZE>::operator=
			(const MyStack/*<T, MAXSIZE>*/ &other) {
	if (this != &other) {
		size = other.size;
		for (int i = 0; i < size; i++)
			data[i] = other.data[i];
	}
	return *this;
}
template<typename T,int MAXSIZE>
T& MyStack<T,MAXSIZE>::top() {
	assert(!empty());
	return data[size - 1];
}
int main() {
	MyStack<int, 10> st1;
	st1.push(1);
	st1.pop();
	MyStack<int, 20> st2;
	st2.empty();
	//st1和st2是同一类型的对象吗？
	//MyStack<int, 10> 和 
	//MyStack<int, 20>实例化的成员函数？
	return 0;
}