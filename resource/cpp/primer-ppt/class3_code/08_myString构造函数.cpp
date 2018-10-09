#include <iostream>
#include <string>
#include <string.h>
using namespace std;
class myString {
public:
	myString() {
		ps = new char[1]; 
		ps[0] = '\0';
	}
	myString(const char *str) {
		int len = strlen(str) + 1;
		ps = new char[len];
		strcpy(ps, str);
	}
	/*
	myString(const char *str = NULL) {
		if (str == NULL) {
			ps = new char[1];
			ps[0] = '\0';
		}
		else {
			int len = strlen(str) + 1;
			ps = new char[len];
			strcpy(ps, str);
		}
	}*/
	~myString() {
		if (ps) delete[] ps;
	}
	const char* c_str()const {
		return ps;
	}
private:
	char *ps;
};
int main() {
	string s1;
	string s2("abc");
	cout << "-" << s1.c_str() << "-" << endl;
	cout << s2.c_str() << endl;
	myString ms1;
	myString ms2("abc");
	cout << "-" << ms1.c_str() << "-" << endl;
	cout << ms2.c_str() << endl;
	return 0;
}