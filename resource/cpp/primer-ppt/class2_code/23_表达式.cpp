#include <iostream>
using namespace std;

int main() {
	//优先级和结合律
	int n = -10 * 2 + 20 / 2 * 3 - 10; //0
	((((-10) * 2) + ((20 / 2) * 3)) - 10); //0
	int arr[] = { 1,4,5,8 };
	int i1 = *arr + 2;   //3
	int i2 = *(arr + 2); //5

	//常用写法:
	int *p = arr;
	*p++; // *(p++), *p 然后 p = p + 1
	*++p; // *(++p) 先 p = p + 1, 然后 *p
	
	//避免下面这样的写法
	i1 = 0;
	cout << (i1++ + ++i1 - 1) << endl;

	return 0;
}