#include <iostream>
using namespace std;
int main() {
	int *p1 = new int[336870912];
	int *p2 = new int[336870912]; 
	//申请失败，抛出异常，不处理就中止程序
	int *p3 = new (std::nothrow)int[336870912];
	//申请失败，p3==NULL，类似C的处理方式
	return 0;
}