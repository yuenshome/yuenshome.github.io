#include <iostream>
using namespace std;
void f1(int a = 0) {}
void f2(double fd,int i=3,char* ps="abc"){}
int main() {
	f1();			//相当于 f1(0)
	f1(10); 
	f2(3.14);		//相当于 f2(3.14,3,"abc")
	f2(3.14, 10);	//相当于f2(3.14,10,"abc")
	f2(3.14, 10, "bcd");
	return 0;
}