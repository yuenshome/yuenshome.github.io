#include <iostream>
#include <cstring>
using namespace std;

template<typename T>
int compare(const T& a, const T& b) {
	cout << "const T&\n";
	if (a < b) return -1;
	if (b < a) return 1;
	return 0;
}
template<typename T>
int compare(T* a, T*  b) { //值传递
	cout << "T* \n";
	if (*a < *b) return -1;
	if (*b < *a) return 1;
	return 0;
}
int compare(const char* const &a, const char* const &b) {
	cout << "const char* const & \n";
	return strcmp(a, b);
}
int compare(const char* &a, const char* &b) {
	cout << "const char* & \n";
	return strcmp(a, b);
}
int main() {
	int a = 20, b = 10;
	cout << compare(a, b) << endl; //const T&
	cout << compare(&a, &b) << endl; // T* 
	//如果没有2个非模板函数,则下面调用 T*
	cout << compare("abc", "abcd") << endl; //const char* const &
	const char *p1 = "abc", *p2 = "abcd";
	cout << compare(p1, p2) << endl; //const char* &
	return 0;
}