#include <iostream>
#include <cstdlib>
using namespace std;
class A {
public:
	A(int i=0) :pi(new int(i)) {cout << "A构造\n";}
	~A() { cout << "A析构！\n";delete pi;}
	static void* operator new(std::size_t size, 
					const char* file, int line){ 
		//跟踪在哪个文件哪行开辟的内存
		cout << __FILE__ << "---" << __LINE__ << endl;
		if (void * p = malloc(size)) {
		cout << "-A operator new\n"; 
		return p; 
		}
		throw bad_alloc(); 
	}
private:
	int* pi;
};
int main() {
	A* pa = new(__FILE__, __LINE__) A(1);
	cout << "-----------------\n";
	delete pa;
	return 0;	
}