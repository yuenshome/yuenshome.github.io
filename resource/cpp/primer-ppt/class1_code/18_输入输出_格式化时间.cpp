#include <iostream>
#include <iomanip>
using namespace std;
struct mytime {
	int h;
	int m;
	int s;
};
int main() {
	mytime t1 = { 7,9,8 };
	cout << "time: " << t1.h
		<< ":" << t1.m
		<< ":" << t1.s
		<< endl;
	cout << "time: " << setfill('0')
		<< setw(2) << t1.h << ":"
		<< setw(2) << t1.m << ":"
		<< setw(2) << t1.s << endl;
	return 0;
}

