///内联函数: .h
class Stack {
public:
	Stack(int len = 1024);
	~Stack();
	bool isEmpty()const {
		return topidx == 0;
	}  //隐式内联
	inline bool isFull()const; //内联
	int get_size()const; //内联
	int top()const;
	void push(int data);
	void pop();
private:
	int *ps;
	int topidx;
	int size;
};
inline bool Stack::isFull()const{
	return topidx == size; 
}
inline int Stack::get_size()const{
	return size; 
}


///.cpp
#include <iostream>
#include "Stack.h"
Stack::Stack(int len): topidx(0), 
					   size(len) {
	ps = new int[len]; 
}
Stack::~Stack() {
	delete[] ps;
	ps = NULL;
}

int Stack::top()const {
	return ps[topidx - 1];
}

void Stack::push(int data) {
	ps[topidx++] = data;
}

void Stack::pop() {
	topidx--;
}


///.main
#include <iostream>
#include "Stack.h"
using namespace std;

int main() {
	Stack S;
	for (int i = 0; i < 5; i++) {
		if (!S.isFull())
			S.push(i);
	}
	cout << S.get_size() << endl;
	while (!S.isEmpty()) {
		cout << S.top() << " ";
		S.pop();
	}
	cout << endl;
	return 0;
}