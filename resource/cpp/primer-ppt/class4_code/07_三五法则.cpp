#include <iostream>
using namespace std;

class A {
public:
	A() = default;
	~A() = default;
	A(const A&) = default;
	A& operator=(const A&) = default;
};
class B {
public:
	B() = default;
	B(const B&) = delete;
	B& operator=(const B&) = delete;
};
class C {
public:
	void destroy() { delete this; }
private:
	~C() {};
};
int main() {
	A a1;
	A a2 = a1;
	B b1;
	//B b2 = b1; //´íÎó£¬¿½±´¹¹Ôìdelete
	//C c1; //´íÎó£¬Îö¹¹º¯ÊıÊÇ private
	C* pc = new C;
	pc->destroy();
	return 0;
}