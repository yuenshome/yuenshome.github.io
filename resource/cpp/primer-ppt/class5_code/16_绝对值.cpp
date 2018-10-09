#include <iostream>
using namespace std;
struct absInt {
	int operator()(int val)const {
		return val > 0 ? val : -val;
	}
};
int main() {
	int a = -2;
	absInt abs;
	cout << abs(a) << endl; // Êä³ö2
	cout << absInt()(-3) << endl; //Êä³ö3
	return 0;
}
