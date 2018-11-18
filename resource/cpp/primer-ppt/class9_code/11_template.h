/// 11_template.h
#ifndef TEMPLATE_H
#define TEMPLATE_H
#include <vector>
template <typename T>
int compare(const T& a, const T& b) {
	if (a < b) return -1;
	if (b < a) return 1;
	return 0;
}
template <typename T>
class my_vector {
public:
	my_vector() { data.resize(10); }
	my_vector(const T& t,int n=10) {
		for (int i = 0; i < n; i++)
			data.push_back(t);
	}
private:
	std::vector<T> data;
};
#endif // !TEMPLATE_H