#include <iostream>
using namespace std;
class Shape { //抽象基类
public:
	//纯虚函数（接口）
	virtual double area()const = 0;
	virtual ~Shape() = default;//虚析构
};
class Rectangle :public Shape {
public:
	Rectangle(double w, double h)
		:width(w), height(h) {}
	virtual double area()const override{
		return width*height;
	}
private:
	double width;
	double height;
};
class Circle :public Shape {
public:
	Circle(double r) :radius(r) {}
	virtual double area()const override {
		return 3.1415926*radius*radius;
	}
private:
	double radius;
};

double get_area(const Shape& sp) {
	return sp.area();
}

int main() {
	Rectangle r1(10.0, 20.0);
	Circle c1(10.0);
	Shape *p = nullptr;
	p = &r1;
	cout << p->area() << endl; //200
	p = &c1;
	cout << p->area() << endl; //314.159
	cout << "------------\n";
	cout << get_area(r1) << endl;//200
	cout << get_area(c1) << endl;//314.159
	return 0;
}

