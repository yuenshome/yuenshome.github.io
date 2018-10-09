#include <iostream>
using namespace std;
class Point {
	friend istream &operator >> (istream& in, Point& a);
	friend ostream &operator<<(ostream& out, const Point& a);
public:
	Point(int _x = 0, int _y = 0)
		:x(_x), y(_y) {}
	Point &operator+=(const Point &a);
private:
	int x, y;
};
Point & Point::operator+=(const Point &a) {
	x += a.x;
	y += a.y;
	return *this;
}



istream &operator >> (istream& in, Point& a) {
	cout << "input Point x y:" << endl;
	in >> a.x >> a.y;
	return in;
}
ostream &operator<<(ostream& out, const Point& a) {
	cout << "(" << a.x << "," << a.y << ") " ;
	return out;
}
int main() {
	int a = 2,b=3;
	a += b += 2; 
	// a += (b += 2)
	//结果： b = 5, a = 7
	cout << b << a << endl; // 5 7
	(a += b) = 10; // a += b 返回 a
	cout << a << endl; //10

	Point p1(1, 2), p2(3, 4),p3(5,9);
	p1 += p2;
	cout << p1 << endl; // (4,6)
	(p1 += p2) = p3; // (p1 += p2) 返回的是 p1的引用
	cout << p1 << endl; // (5,9)
	p1 += p2 = p3; //右结合律，p1 += (p2 = p3)
	cout << p1 << endl; // (10,18)
	return 0;
}