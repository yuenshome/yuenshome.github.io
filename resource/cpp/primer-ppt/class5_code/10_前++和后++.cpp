#include <iostream>
using namespace std;
class Point {
	friend istream &operator >> (istream& in, Point& a);
	friend ostream &operator<<(ostream& out, const Point& a);
	//friend Point &operator++(Point &a);
	//friend Point operator++(Point &a, int);
public:
	Point(int _x = 0, int _y = 0)
		:x(_x), y(_y) {}
	Point &operator++();
	Point operator++(int);
private:
	int x, y;
};
Point & Point::operator++() {
	x += 1;
	y += 1;
	return *this;
}
/*const*/Point Point::operator++(int) {
	Point tmp = *this;
	x += 1;
	y += 1;
	//上面2句也可写为： ++*this;
	return tmp;
}

//Point &operator++(Point &a) {
//	a.x += 1;
//	a.y += 1;
//	return a;
//}
//Point operator++(Point &a, int) {
//	Point tmp = a;
//	a.x += 1;
//	a.y += 1;
//	return tmp;
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
	Point p1(1, 2);
	cout << p1++ << endl; //(1,2)
	cout << ++p1 << endl; //(3,4)
	return 0;
}