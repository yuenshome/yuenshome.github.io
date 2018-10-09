#include <iostream>
#include <cstring>
using namespace std;
class myString {
public:
	myString(const char * pstr = NULL) {
		if (!pstr) {
			ps = new char[1];
			ps[0] = '\0';
		}
		else {
			ps = new char[strlen(pstr) + 1];
			strcpy(ps, pstr);
		}
	}
	myString(const myString &other):ps(new char[strlen(other.ps) + 1]) {
		strcpy(ps, other.ps);
	}
	myString &operator=(const myString &other) {
		if (this != &other) {
			delete[] ps;
			ps = new char[strlen(other.ps) + 1];
			strcpy(ps, other.ps);
		}
		return *this;
	}
	~myString() {
		delete[] ps;
	}
private:
	char *ps;
};
int main() {
	myString s1 = "abc";
	myString s2 = s1;
	myString s3;
	s3 = s1;
	return 0;
}