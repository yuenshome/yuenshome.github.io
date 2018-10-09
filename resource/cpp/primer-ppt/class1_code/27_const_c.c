#include <stdio.h>
int main() {
	const int i = 10;
	//const int i;	//错误，const变量必须在定义时初始化
	//i=100;		//错误，const类型不能修改
	int *p = &i;	//将i的地址赋值给指针p( 在C中ok)
	*p = 20;		//通过指针修改const int i的值
	printf("i=%d,*p=%d\n", i, *p); // 20 20
	return 0;
}