///stringstream
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

int main() {
	//istringstream 从string中读取数据
	//ostringstream 向string写入数据
	//stringstream 即可以读，也可以写
	//基本操作和前面的类似(继承体系类似)

	string str1("a:111111\nb:222222\nc:333333\n");
	stringstream ss1;
	stringstream ss2(str1); //保存str1的拷贝

	ss1 << 1 << " " << 2 << " I like C++" << endl;
	int i1, i2;
	string tmps;
	ss1 >> i1 >> i2 >> tmps;
	cout <<i1<<" "<<i2<<" "<<tmps<<endl; //与fstream一样的效果
	ss1.seekg(0, stringstream::beg);
	getline(ss1, tmps); //ok,字符串没有截断
	cout << tmps << endl;
	cout << "1-------------\n";

	//将string流中的内容 复制到 string对象
	string str2 = ss1.str(); //ss1流中的数据 拷贝到了str2
	cout << str2; 
	cout << "2-------------\n";
	//改变string流中的内容
	ss1.str(str1); //用str1的内容拷贝到 ss1流中
	ss1.seekg(0, stringstream::beg);
	while(getline(ss1, tmps)) 
		cout << tmps << endl;
	cout << "3-------------\n";
	//ss1.clear() 不是清空流数据，而是流状态复位
	//要清空数据，可以这样写：
	ss1.str("");

	//二进制的read write也可以 
	struct Student { char name[20];	int age; char sex; };
	Student stu[] = { { "张三",10,'m' },{ "李四",20,'f' },{ "王五",0,'m' } };

	ostringstream  os1;
	os1.write((char*)stu, sizeof(stu));
	istringstream is1(os1.str());
	Student tmp;
	for (int i = 0; i < 3; ++i) {
		is1.read((char*)&tmp, sizeof(tmp));
		cout << tmp.name << ":" << tmp.age << ":" << tmp.sex << endl;
	}

	cout << "end--------------\n";
	return 0;
}
