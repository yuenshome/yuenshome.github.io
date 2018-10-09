#include <iostream>
using namespace std;

class Point {
	friend ostream& operator<<(ostream&, const Point&);
public:
	explicit Point(int a = 0, int b = 0) :x(a), y(b) {}
	Point operator+(const Point& rhs) {
		int tmp_x = x + rhs.x;
		int tmp_y = y + rhs.y;
		return Point(tmp_x, tmp_y);
	}
	/*explicit*/ operator int()const {return x;}
private:
	int x;
	int y;
};

ostream& operator<<(ostream& out, const Point& p) {
	out << "(" << p.x << "," << p.y << ")\n";
	return out;
}

int main() {
	Point p1(1, 2);
	cout << (p1 + 3) << endl;
	return 0;
}