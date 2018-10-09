#include <iostream>
using namespace std;
class Date {
public:
	void init() {
		while (1) {
			cout << "请输入年 月 日:" << endl;
			cin >> y >> m >> d;
			if (isValid()) break;
			cout << "输入错误，重新输入！" << endl;
		}
	}
	void print() {
		cout << y << "-" << m << "-" << d << endl;
	}
private:
	bool isValid() {
		if (y > 0 && y < 9999 && m>0 &&
			m < 13 && d>0 && d < 32)
			return true;
		return false;
	}
	int y, m, d;
};
int main() {
	Date d1;
	d1.init();
	d1.print();
	return 0;
}