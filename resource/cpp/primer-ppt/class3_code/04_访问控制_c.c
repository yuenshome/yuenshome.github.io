///c格式，struct
#include <stdio.h>
typedef struct _Date {
	int y, m, d;
}Date;
int isValid(Date *pd) {
	if (pd->y > 0 && pd->y < 9999 &&
		pd->m >0 &&	pd->m < 13 && 
		pd->d>0 && pd->d < 32)
		return 1;
	return 0;
}
void init(Date *pd) {
	while (1) {
		printf("请输入年 月 日:\n");
		scanf("%d%d%d", &pd->y, &pd->m, &pd->d);
		if (isValid(pd)) break;
		printf("输入格式错误，重新输入！\n");
	}
}
void print(Date *pd) {
	printf("%d-%d-%d\n", pd->y, pd->m, pd->d);
}
int main()
{
	Date d1;
	init(&d1);
	print(&d1);
	return 0;
}