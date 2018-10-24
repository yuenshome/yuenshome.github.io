#include <iostream>
using namespace std;
class Bird {
public:
	void eat() { cout << "鸟吃东西" << endl; }
};
class FlyingBird :public Bird {
public:
	void fly() { cout << "鸟飞行" << endl; }
};
class Qie :public Bird { //is-a
public:
	//somecode...
};
int main() {
	Qie q;
	//企鹅不会飞，这里也不能调用，ok
	//q.fly(); //没有fly函数
	//其他会飞的鸟 可以 public继承 FlyingBird
	return 0;
}