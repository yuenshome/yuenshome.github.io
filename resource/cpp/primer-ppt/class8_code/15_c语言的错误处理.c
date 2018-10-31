/// .c
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int my_fun1(int i) {
	if (i >= 0) {
		//do something...
		return 1;
	}
	//do something...
	return 0;
}

char* my_malloc1(int size) {
	char* p = (char*)malloc(size);
	return p;
}

int error_no = 1;
double triangle_area1(double a, double b, double c) {
	if (!(a + b > c && a + c > b && b + c > a)) {
		error_no = 0;
		return 0.0;
	}
	double p = (a + b + c) / 2;
	double area = sqrt(p*(p - a)*(p - b)*(p - c));
	error_no = 1;
	return area;
}

int main() {
	//1.利用返回值来确定是否正确执行
	int ret = my_fun1(10);
	if (0 == ret) {
		//失败了，处理代码...
	}
	else {
		//成功了，处理代码...
	}
	//2.返回值用于判断是否成功，也有其本身作用
	char* pc1 = my_malloc1(100);
	if (!pc1) {
		//没成功，处理代码...
	}
	//成功了，继续执行...
	//3.用全局变量 error_no来判断错误
	double s1 = triangle_area1(3, 4, 5);
	if (0 == error_no)
		printf("三角形三边长度关系不正确！\n");
	else
		printf("面积=%f\n", s1);
	s1 = triangle_area1(2, 2, 4);
	if (0 == error_no)
		printf("三角形三边长度关系不正确！\n");
	else
		printf("面积=%f\n", s1);
	return 0;
}