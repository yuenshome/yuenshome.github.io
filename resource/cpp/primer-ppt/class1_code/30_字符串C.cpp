#include <stdio.h>
#include <string.h>
int main() {
	//字符数组
	char str1[20] = "abcde";		//初始化
	char str2[20] = { 'a','b','c' };//初始化
	//str2 = "abc";	错误
	char str3[20];
	str3[0] = 'a'; str3[1] = 'b'; str3[2] = '\0';
	//字符指针
	char *pstr = "bcd"; //将常量字符串的地址赋给pstr
	pstr = "def";
	pstr = str1;
	pstr[0] = 'x';		//通过指针修改
	*(pstr + 1) = 'y';	//通过指针修改
	printf("str1=%s\n", str1); // 输出xycde
	//字符串长度
	printf("str1长度= %d\n", strlen(str1));  //5
	//字符串拷贝
	printf("str1=%s\n", strcpy(str1, "ddd"));//ddd
	//字符串连接
	printf("str1=%s\n", strcat(str1, str2)); //dddabc
	//字符串比较
	if (strcmp(str2, str3) > 0)
		printf("%s > %s\n", str2, str3);
	else if(strcmp(str2, str3) == 0)
		printf("%s == %s\n", str2, str3);
	else
		printf("%s < %s\n", str2, str3);
	//字符串查找
	strcpy(str2, "--ab=="); //str3: "ab"
	printf("%s\n", strstr(str2, str3)); //ab==
	return 0;
}