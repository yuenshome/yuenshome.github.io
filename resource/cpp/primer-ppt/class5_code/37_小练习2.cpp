#include <iostream>
#include <functional>
#include <algorithm>
#include <vector>
#include <memory>
using namespace std;
shared_ptr<vector<int>> new_vector() {
	return make_shared<vector<int>>();
}
void input_vector(shared_ptr<vector<int>> pv) {
	//注意值传递和引用传递的区别
	cout << pv.use_count() << endl; 
	int v;
	while (cin >> v)
		pv->push_back(v);
}
ostream& print(ostream& out, const int& v, char sep) {
	return out << v << sep;
}
int main() {
	shared_ptr<vector<int>> pv = new_vector();
	input_vector(pv);
	//以 ' ' 作为分隔符打印输出
	for_each(pv->begin(), pv->end(), 
	  bind(print, ref(cout),std::placeholders::_1,' '));
	cout << endl;
	return 0;
}