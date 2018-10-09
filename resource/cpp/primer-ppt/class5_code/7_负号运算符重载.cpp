#include <iostream>
using namespace std;
class Point {
	friend istream &operator >> (istream& in, Point& a);
	friend ostream &operator<<(ostream& out, const Point& a);
	//friend Point operator-(const Point& a);
public:
	Point(int _x = 0, int _y = 0)
		:x(_x), y(_y) {}
	/*const*/ Point operator-()const;
private:
	int x, y;
};
/*const*/ Point Point::operator-()const {
	return Point(-x, -y);
}
//Point operator-(const Point& a) {
//	return Point(-a.x, -a.y);
//}
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
	Point p1(10,20);
	cout << p1 << endl;
	cout << -p1 << endl;
	//假如返回的是 const，则必须用const引用接收
	/*const*/ Point &p2 = -p1;
	Point p3 = -p1; //值拷贝就无所谓
	cout << p2;
	return 0;
}
