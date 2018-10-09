#include <iostream>
using namespace std;
namespace A {
	void f1(int a, int b) {}
}
namespace B {
	void f1(int a) {}
}
using A::f1;
using B::f1;
int main() {
	//A中的f1和B中的f1构成重载吗?
	f1(10);
	f1(10, 20);
	return 0;
}

