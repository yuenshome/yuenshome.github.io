#include <iostream>
#include <string>
using namespace std;
int main() {
	//std::string
	std::string str1("abc"); //初始化
	string str2 = "bcd";	 //初始化
	str2 = "defg";			 //可以直接赋值
	str2 = str1;			 //可以直接赋值
	
	const char *pstr = str2.c_str(); //转c风格字符串
	str2[0] = 'X';		//可以直接下标访问操作
	str2.at(1) = 'Y';	//可以 at 访问操作
	cout <<"str2=" << str2 << endl; //XYc
	
	//求字符串长度
	cout << str2.size() << endl;
	cout << str2.length() << endl;
	//strlen(str1); 错误
	cout << strlen(str2.c_str()) << endl; //正确
	//字符串连接
	str2 = str2 + str1 + "!!";
	cout << "str2=" << str2 << endl; //XYcabc!!
	//字符串比较 (str1: abc)
	cout << str2.compare(str1) << endl; //-1
	cout << (str2 < str1) << endl;	    //1
	//字符串查找
	cout << str2.find(str1) << endl;	//3
	//字符串提取
	string str3 = str2.substr(3, 3);
	cout << str3 << endl;				//abc

	return 0;
}