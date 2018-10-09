#include <iostream>
#include <string.h>
using namespace std;
class A {
public:
	A(int num) { this->num = num; }
	A& add(int n) { num += n; return *this; }
	int get_num()const { return num; }
private:
	int num; 
};
int main() {
	A a1(1);
	a1.add(2).add(3).add(4).add(5);
	cout << a1.get_num() << endl; //15
	return 0;
}