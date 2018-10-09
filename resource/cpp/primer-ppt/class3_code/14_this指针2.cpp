#include <iostream>
#include <string.h>
using namespace std;
class A {
public:
	A(int num) { this->num = num; }
	int show()const { return num; }
private:
	int num; 
};
int main() {
	A a1(20);
	cout << a1.show() << endl; //20
	A a2(1);
	cout << a2.show() << endl; //1
	return 0;
}
