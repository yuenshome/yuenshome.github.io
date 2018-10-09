#include <iostream>
#include <string>
#include <utility>
using namespace std;

int main() {
	string *ps = new string[100];
	string s;
	string *q = ps;
	while (cin >> s && s!="quit" && q != ps + 100) {
		*q++ = std::move(s);
	}
	delete[] ps;
	return 0;
}