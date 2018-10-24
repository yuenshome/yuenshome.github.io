#include <iostream>
using namespace std;
class Bird {
public:
	void eat() { cout << "鸟吃东西" << endl; }
	void fly() { cout << "鸟飞行" << endl; }
};
class Qie { //根据Bird实现出
public:
	void eat() { bird.eat(); }
private:
	Bird bird;
};
int main() {
	Qie q;
	q.eat();
	//q.fly(); //不行
	return 0;
}