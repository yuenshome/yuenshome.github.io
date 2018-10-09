#include <iostream>
using namespace std;
void fun(int *i) 
  { cout << 1 << endl; } //1
void fun(const int *i) 
  { cout << 2 << endl; } //2
void fun(int &i)
  { cout << 3 << endl; } //3
void fun(const int &i) 
  { cout << 4 << endl; } //4
int main() {
	int i = 10;
	int &ri = i;
	const int &cri = i;
	//&ri, &cri, ri, cri为实参调用fun
	fun(&ri);  //1
	fun(&cri); //2
	fun(ri);   //3
	fun(cri);  //4
	fun(42);   //4
	fun(1.23); //4
	fun('a');  //4
	return 0;
}