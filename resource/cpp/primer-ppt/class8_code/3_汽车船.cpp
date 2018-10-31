///汽车船
#include <iostream>
using namespace std;

using Engine = int;
using Wheel = int;
using Propeller = int; 
class vehicle { //车 类
public:
	void run() { cout << "车在马路上跑\n"; }
private:
	Engine engine; //引擎
	Wheel wheel;   //轮子
};
class ship { //船 类
public:
	void swim() { cout << "船在水中漂\n"; }
private:
	Engine engine;		 //引擎
	Propeller propeller; //螺旋桨
};
class Carship :public vehicle, public ship {
private:
	int special;
};
int main() {
	Carship cs;
	cs.run();
	cs.swim();
	return 0;
}