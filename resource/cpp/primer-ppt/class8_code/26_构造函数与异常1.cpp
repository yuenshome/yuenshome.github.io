#include <iostream>
using namespace std;
void* operator new(std::size_t size) {
	cout << "malloc memory!\n";
	void* p = malloc(size);
	if (p) return p;
	throw bad_alloc();
}
void operator delete(void* p) {
	cout << "free memory!\n";
	free(p);
}
struct A1 {
	A1() { cout << "A1()\n"; }
	~A1() { cout << "~A1()\n"; }
};
struct A2 {
	A2() { cout << "A2()\n"; }
	~A2() { cout << "~A2()\n"; }
};
struct A3 {
	A3() { throw 1; cout << "A3()\n"; }
	~A3() { cout << "~A3()\n"; }
};
class Object {
public:
	Object() { cout << "Object()\n"; }
	~Object() { cout << "~Object()\n"; }
private:
	A1 a1;
	A2 a2;
	A3 a3;
};

int main() {
	//obj构造失败,异常发生在构造过程中
	try { 
		Object obj;
	}
	catch (...) { cout << "error\n"; }
	cout << "------------\n";
	try { 
		Object* pobj = new Object;
	}
	catch (...) { cout << "error\n"; }
	return 0;
}