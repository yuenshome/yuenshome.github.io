#include <iostream>
#include <cmath>
using namespace std;

class Point;		//前向声明
class ManagerPoint{ //管理Point的类
public:
	double distance(Point &a, Point &b);
};

class Point{		//Point类
public:
	friend double ManagerPoint::distance(Point &a, Point &b);
	Point(double x, double y):x(x),y(y)	{ }
	void print()const;
	const double &get_x()const { return x; }
	const double &get_y()const { return y; }
private:
	double x, y;
};

void Point::print()const {
	cout << "(" << x << "," << y << ")" << endl;
}

double ManagerPoint::distance(Point &a, Point &b) {
	double dx = a.x - b.x;
	double dy = a.y - b.y;
	return sqrt(dx*dx + dy*dy);
}

int main(){
	Point p1(1.0, 1.0), p2(4.0, 5.0);
	p1.print();
	p2.print();
	ManagerPoint mp;
	double d = mp.distance(p1, p2);
	cout << "Distance = " << d << endl;
	return 0;
}