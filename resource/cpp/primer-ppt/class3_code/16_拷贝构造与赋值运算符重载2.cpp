#include <iostream>
#include <string>
using namespace std;

class A {
public:
	A(int n = 0) :num(n) {}
	A(const A &other) :num(other.num){  }
	A& operator=(const A &other) {
		num = other.num;
		return *this;
	}
private:
	int num; 
};
int main() {
	A a1;
	A a2;
	a2 = a1; //¸³Öµ
	return 0;
}