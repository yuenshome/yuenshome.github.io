#include <stdio.h>
#include <stdlib.h>
int a = 0, b, c = 10; // a,b 是data区bss段，c是data区rw段
extern int x;		  //变量的声明，没有分配内存
static char ch1, ch2 = 'o';//ch1是data区bss段，ch2是data区rw段
struct A {            //结构体类型定义，不分配内存
	int data;
	int *p_data;
};
int main() {
	char d = 'x';	//栈区
	static int e;	//data区bss段
	char *p = (char *)malloc(20);//p在栈区，p指向的内存在堆区
	A a1;				  //栈区
	a1.p_data = &a1.data; //指针本身 和 它指向的内存 都在 栈区
	A* pa2 = (A*)malloc(sizeof(A));//pa2在栈区，*pa2在堆区
	pa2->p_data = (int*)malloc(10 * sizeof(int)); //都在堆区
	free(pa2->p_data);
	free(pa2);
	return 0;
}