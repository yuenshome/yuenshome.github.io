#include <iostream>
using namespace std;

//计算总和：尝试(有什么问题?)
template<typename T>
T sum_test(T* beg, T* end) {
	T total = T(); // = 0 ?
	while (beg != end) {
		total += *beg;
		++beg;
	}
	return total;
}

template<typename T> struct base_traits;
template<> struct base_traits<int> {
	typedef long long sum_type;
	static sum_type zero() { cout << "int_zero\n"; return 0; }
};
template<> struct base_traits<char> {
	typedef int sum_type;
	static sum_type zero() { cout << "char_zero\n"; return 0; }
};

template<typename T>
typename base_traits<T>::sum_type sum_test_new(T* beg, T* end) {
	typename base_traits<T>::sum_type total = base_traits<T>::zero();
	while (beg != end) {
		total += *beg;
		++beg;
	}
	return total;
}


int main() {
	int num[] = { 1,2,3,4 };
	cout << sum_test(&num[0], &num[sizeof(num)/sizeof(int)]) << endl;
	char name[] = "abc"; //97 98 99 = 294 - 256 = 38(&的ascii)
	cout << sum_test(&name[0], &name[sizeof(name)-1]) << endl; //&
	cout << "------\n";
	cout << sum_test_new(&num[0], &num[sizeof(num) / sizeof(int)]) << endl;
	cout << sum_test_new(&name[0], &name[sizeof(name) - 1]) << endl; //294
	return 0;
}