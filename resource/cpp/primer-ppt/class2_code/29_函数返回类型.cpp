#include <iostream>
using namespace std;
int f1() { 
	int i = 10;
	return i; 
} 
int *f2() {
	int *p = new int[3]{ 1,2,3 };
	return p;
}
int &f3() {
	int *p = new int(100);
	return *p;
}
int main() {
	int i1;
	i1 = f1();
	cout << i1 << endl; //10
	int *pi2;
	pi2 = f2(); 
	cout << *++pi2 << endl; //2
	int &ri3 = f3(); 
	cout << ri3 << endl; //100
	delete[](--pi2);
	delete &ri3;
	return 0;
}