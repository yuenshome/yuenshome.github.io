#include <iostream>
using namespace std;
class Stack {
public:
	Stack() { //无参构造函数
		ps = new int[1024];
		size = 1024;
		topidx = 0;
		cout << "Stack() run" << endl;
	}
	Stack(int len) { //带参构造函数
		ps = new int[len];
		size = len;
		topidx = 0;
		cout << "Stack(int len) run" << endl;
	}

	/*Stack(int len = 1024) {
	ps = new int[len];
	size = len;
	topidx = 0;
	}*/
	~Stack() {
		if (ps) delete[] ps;
		ps = NULL;
		cout << "析构函数run..." << endl;
	}
	bool isEmpty()const { return topidx == 0; }
	bool isFull()const { return topidx == size; }
	int top()const {
		return ps[topidx - 1];
	}
	void push(int data) {
		ps[topidx++] = data;
	}
	void pop() {
		topidx--;
	}
private:
	int *ps;
	int topidx;
	int size;
};
int main() {
	Stack S1;      //调用无参构造 不能写 Stack S1();
	Stack S2(100); //调用带参构造
	Stack S3{ 10 };//调用带参构造
	Stack *p1 = new Stack; //无参构造
	Stack *p2 = new Stack(10);//带参构造
	Stack *p3 = new Stack{ 10 };//带参构造
								//对照：
	int a1; //不能写 int a1(); 这是函数声明
	int a2(10);
	int a3{ 10 };
	int *pa1 = new int;
	int *pa2 = new int(10);
	int *pa3 = new int{ 20 };
	return 0;
}