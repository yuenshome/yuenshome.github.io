#include <iostream>
#include <string.h>
using namespace std;
class B {
public:
	B(double f) :fd(f), num(fd) {}
	int get_num()const { return num; }
private:
	int num;
	double fd;
};
int main() {
	B b(1.2);
	cout << b.get_num() << endl;
	//没有得到预期中的 1
	return 0;
}