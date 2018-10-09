#include <iostream>
#include <cstring>
using namespace std;
class myString {
	friend ostream &operator<<(ostream& out, const myString& a);
public:
	myString(const char* _ps = NULL) {
		if (!_ps)
			ps = new char[1]{ '\0' };
		else {
			ps = new char[strlen(_ps) + 1];
			strcpy(ps, _ps);
		}
	}
	char &operator[](int i) {	return ps[i];	}
	const char &operator[](int i)const { return ps[i]; }
private:
	char *ps;
};
ostream &operator<<(ostream& out, const myString& a) {
	out << a.ps;
	return out;
}

int main() {
	myString s1("abc");
	s1[1] = 'B';
	cout << s1 << endl; //aBc
	const myString s2("abc");
	cout << s2[1] << endl; //b
	return 0;
}