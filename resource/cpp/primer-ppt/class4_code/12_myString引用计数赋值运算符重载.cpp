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
	myString &operator=(const myString &other) {
		if (this != &other) {
			(*count)--; //本对象原来的计数器 -1
			if (*count == 0) {
				cout << "delete in = ." << endl;
				delete[] ps;
				delete count;
			}
			ps = other.ps;
			count = other.count;
			(*count)++;
		}
		return *this;
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
	myString s1("abc");
	myString s2 = s1; //拷贝构造
	cout << s1.get_count() << endl; //2
	myString s3("123");
	myString s4 = s3;
	cout << s3.get_count() << endl; //2
	s3 = s2;
	cout << s3.get_count() << endl; //3
	cout << s4.get_count() << endl; //1
	cout << "============" << endl;
	return 0;
}