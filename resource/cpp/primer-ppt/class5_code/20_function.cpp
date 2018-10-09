#include <iostream>
#include <vector>
#include <functional>
using namespace std;

int add(int a, int b) { return a + b; }
class sub {
public:
	int operator()(int a, int b) {
		return a - b;
	}
};
//function用法
int main() {
	function<int(int, int)> f1 = add;
	function<int(int, int)> f2= sub();
	function<int(int, int)> f3 = 
		[](int a, int b) {return a*b; };
	cout << sub()(3, 2) << endl; //1
	//cout << f2(10, 1) << endl;   //9
	cout << f3(2, 4) << endl;    //8
	
	cout << "-----------\n";
	vector<int(*)(int, int)> vec1;
	vec1.push_back(add);
	//vec1.push_back(sub()); //不行了
	vec1.push_back([](int a, int b) {return a * b; }); 

	vector<function<int(int, int)>> vec2;
	vec2.push_back(add);
	vec2.push_back(sub()); 
	vec2.push_back([](int a, int b) {return a * b; }); 
	cout << vec2[0](3, 5) << endl; // 3+5 =8
	cout << vec2[1](3, 5) << endl; // 3-5 =-2
	cout << vec2[2](3, 5) << endl; // 3*5 =15
	return 0;
}