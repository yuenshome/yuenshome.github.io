#include <stdio.h>

int main() {
	int a = 12345;
	double f = 123.4567;
	//默认输出整数
	printf("a=%d===\n", a);
	//占8格，右对齐
	printf("a=%8d===\n", a);
	//占8格，左对齐
	printf("a=%-8d===\n", a);
	//默认输出浮点数，小数显示6位
	printf("f=%f===\n", f);
	//占10格，小数显示2位，右对齐
	printf("f=%10.2f===\n", f);
	//占10格，小数显示2位，左对齐
	printf("f=%-10.2f===\n", f);

	return 0;
}

