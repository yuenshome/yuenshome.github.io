#include <iostream>
using namespace std;
class Bird {
public:
	void eat() { cout << "鸟吃东西" << endl; }
	void fly() { cout << "鸟飞行" << endl; }
};
class Qie :public Bird { //is-a
public:
	//somecode...
};
int main() {
	Qie q;
	//企鹅不会飞，这里却可以调用，设计不合理
	q.fly(); 
	return 0;
}