#include <iostream>
using namespace std;
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
	Object() :a2(nullptr),a3(nullptr){
		cout << "Object()\n";
		//a2 = new A2;
		//a3 = new A3; 
		try {
			a2 = new A2;
			a3 = new A3;
		}
		catch (...) {
			delete a2;
			delete a3;
			cout << "a2,a3 delete!\n";
			throw;
			//确保构造失败时，不会泄露内存
		}
	} 
	~Object() {
		cout << "~Object()\n"; 
		delete a2; delete a3;
	}
private:
	A1 a1;
	A2 *a2;
	A3 *a3;
};

int main() {
	try { 
		Object obj;
	}
	catch (...) { cout << "error\n"; }
	return 0;
}