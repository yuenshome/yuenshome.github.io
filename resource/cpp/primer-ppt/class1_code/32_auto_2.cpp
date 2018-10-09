#include <iostream>
#include <vector>
#include <string>
using namespace std;

int main() {
	vector<string> vs =
		{ "all","people","like","c++" };

	for (vector<string>::iterator i = 
			     vs.begin(); i != vs.end(); i++)
		cout << *i << " ";
	cout << endl;

	for (auto i = vs.begin(); i != vs.end(); i++) 
		cout << *i << " ";
	cout << endl;

	for (auto &s : vs)
		cout << s << " ";
	cout << endl;

	return 0;
}