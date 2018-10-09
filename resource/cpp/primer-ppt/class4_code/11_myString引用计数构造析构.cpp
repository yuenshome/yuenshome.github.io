#include <iostream>
#include <cstring>
using namespace std;
class myString {
public:
	myString(const char * pstr = NULL) {
		if (!pstr) 
			ps = new char[1]{'\0'};
		else {
			ps = new char[strlen(pstr) + 1];
			strcpy(ps, pstr);
		}
		count = new int(1); //构造时开辟空间并置为1
	}
	myString(const myString &other) 
		:ps(other.ps), count(other.count) {
		(*count)++; //拷贝构造时 count + 1
	}
	~myString() { 
		(*count)--; //析构时 count - 1
		if (*count == 0) { //count为 0 释放内存
			delete[] ps;
			delete count;
			cout << "delete!" << endl;
		}
	}
	int get_count()const { return *count; }
private:
	char *ps;
	int *count;
};

int main() {
	myString s1 = "abc";
	cout << s1.get_count() << endl; //1
	myString s2 = s1;
	cout << s2.get_count() << endl; //2
	cout << s1.get_count() << endl; //2
	{
		myString s3 = s2;
		cout << s3.get_count() << endl; //3
	}
	cout << s2.get_count() << endl; //2
	cout << "============" << endl;
	return 0;
}