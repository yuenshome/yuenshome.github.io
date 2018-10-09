#include <iostream>
using namespace std;
class Stack {
public:
	bool isEmpty()const { return topidx == 0; }
	bool isFull()const { return topidx == size; }
	void init(int len = 1024) {
		ps = new int[len];
		size = len;
		topidx = 0;
	}
	void destroy() {
		if (ps) delete[] ps;
		ps = NULL;
	}
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
	Stack S;
	S.init();
	for (int i = 0; i < 5; i++) {
		if (!S.isFull())
			S.push(i);
	}
	while (!S.isEmpty()) {
		cout << S.top() << " ";
		S.pop();
	}
	//S.topidx = 10; //´íÎó£¬ÎÞ·¨ÐÞ¸Ä
	S.destroy();
	return 0;
}