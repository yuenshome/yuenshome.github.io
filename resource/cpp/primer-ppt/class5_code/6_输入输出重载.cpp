#include <iostream>
using namespace std;
class Point {
	friend istream &operator >> (istream& in, Point& a);
	friend ostream &operator<<(ostream& out, const Point& a);
public:
	Point(int _x = 0, int _y = 0)
		:x(_x), y(_y) {}
	void print() {
		cout << "(" << x << "," << y << ")" << endl;
	}
private:
	int x, y;
};
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
	Point p1,p2(100,200);
	cin >> p1;
	p1.print();
	cout << p1 << endl; //和上面效果一样
	cout << p1 << p2 << endl; //还可以这样	
	return 0;
}