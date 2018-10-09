#include <iostream>
using namespace std;
int main() {
	const int i = 10;
	//int *pi = &i; 编译错误(C++中不行)
	const int *pi = &i;	//ok
	//*pi = 20; 编译错误(指向常量的指针无法修改常量)

	//下面尝试，强转转换来修改常量：
	int *pi2 = (int*)&i; //将常量i的地址强转为int *
	*pi2 = 20;	 //将pi2指针指向的地址内容修改为20
	//观察 *pi2 和 i 对应的内存地址是否一样：
	cout << "pi2=" << pi2 << " &i=" << &i << endl;
	//观察 *pi2 和 i 的值
	cout << "*pi2=" << *pi2 << " i=" << i << endl;
	//输出 *pi2=20 i=10
	//思考：为什么会出现这样的结果？？

	//一定要这么做，怎么做？
	volatile const int ii = 10; //使用volatile关键字
	int *pii = (int*)&ii;
	*pii = 20;
	cout << "*pii=" << *pii << " ii=" << ii << endl;
	//输出 *pii=20 ii=20

	return 0;
}