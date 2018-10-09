#include <iostream>
#include <vector>
using namespace std;
int main() {
	vector<int> vs;
	cout << "size=" << vs.size() << " capacity=" << vs.capacity() << endl;
	for (int i = 0; i < 10; i++) {
		vs.push_back(i);
		cout << "size=" << vs.size() << " capacity=" << vs.capacity() << endl;
	}
	cout << "====================" << endl;
	vector<int> vs1;
	vs1.reserve(4);
	cout << "size=" << vs1.size() << " capacity=" << vs1.capacity() << endl;
	for (int i = 0; i < 4; i++) {
		vs1.push_back(i);
		cout << "size=" << vs1.size() << " capacity=" << vs1.capacity() << endl;
		cout << &vs1[0] << endl; //观察连续内存的起始地址
	}
	vs1.push_back(4);
	cout << "size=" << vs1.size() << " capacity=" << vs1.capacity() << endl;
	cout << &vs1[0] << endl; //观察连续内存的起始地址

	return 0;
}