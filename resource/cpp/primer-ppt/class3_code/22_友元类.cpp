#include <iostream>
using namespace std;

class Point {
public:
	friend class Stack; //ÓÑÔªÀà
	Point(double x = 0.0, double y = 0.0) 
		:x(x), y(y) {}
private:
	double x, y;
};

class Stack {
public:
	Stack(int len = 1024);
	~Stack();
	bool isEmpty()const;
	bool isFull()const; 
	const Point &top()const;
	void push(const Point& data) {
		ps[topidx++] = data;
		cout << "(" << data.x << "," 
			<< data.y << ")" << endl;
	}
	void pop();
private:
	Point *ps;
	int topidx;
	int size;
};
inline bool Stack::isFull()const {
	return topidx == size;
}

Stack::Stack(int len) : topidx(0),
size(len) {
	ps = new Point[len];
}
Stack::~Stack() {
	delete[] ps;
	ps = NULL;
}
bool Stack::isEmpty()const {
	return topidx == 0;
}
const Point& Stack::top()const {
	return ps[topidx - 1];
}

void Stack::push(const Point &data) {
	ps[topidx++] = data;
	cout << "(" << data.x << "," << data.y << ")" << endl;
}

void Stack::pop() {
	topidx--;
}
int main(){
	Point p1(1, 1);
	Stack S;
	S.push(p1);
	return 0;
}