#include <iostream>
using namespace std;
class Point {
	friend istream &operator >> (istream& in, Point& a);
	friend ostream &operator<<(ostream& out, const Point& a);
	friend Point operator+(const Point& a,const Point& b);
public:
	Point(int _x = 0, int _y = 0)
		:x(_x), y(_y) {}
	//Point operator+(const Point &a)const;
private:
	int x, y;
};
//Point Point::operator+(const Point &a)const {
//	return Point(x + a.x, y + a.y);
//}
Point operator+(const Point& a,const Point& b) {
	return Point(a.x + b.x, a.y + b.y);
}
Point operator+(const Point& a, int i) {
	return a + Point(i, 0);
}
Point operator+(int i, const Point& a) {
	return a + Point(i, 0);
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
	Point p1(1,2),p2(3,4);
	cout << 10 + p1 + p2 << endl; //(14,6)
	return 0;
}