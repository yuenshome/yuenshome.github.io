#include <iostream>
#include <vector>
#include <string>
using namespace std;
int main() {
	//std::vector的结构
	std::vector<int> vec11; // [ 1, 3, 9 ...]
	vector<string> vec22;   // [ "abc", "play", "C++" ]
	vector<vector<int>> vec33; // [ [1,3,9..],[2,3,4..], ... ]
	vector<vector<string>> vec44; // [ ["hello","C",..],["C++","abc",..],... ]

	//vector的初始化
	vector<int> vec1 = { 1,2,3 };
	vector<int> vec2{ 1,2,3 };  //列表初始化
	vector<int> vec3 = vec1;    //vec1拷贝给 vec3
	vector<int> vec4(10);		//初始化10个元素，每个元素都是0
	vector<int> vec5(10, -1);	//初始化10个元素，每个元素都是-1
	vector<string> vec6(10, "hi"); //初始化10个元素，每个元素都是 "hi"

	//判断是否为空
	cout << vec1.empty() << endl; //0
	//元素个数
	cout << vec1.size() << endl;  //3
	//添加元素在最后面
	vec1.push_back(100);
	cout << vec1[vec1.size() - 1] << endl; //100
	//弹出元素在最后面
	vec1.pop_back();
	cout << vec1[vec1.size() - 1] << endl; //3
	//直接下标访问元素
	cout << vec1[1] << endl; //2
	vec1[1] = 10;
	cout << vec1[1] << endl; //10
	// vector<string> vec6(10, "hi")
	vec6[0][1] = 'X';
	cout << vec6[0] << endl; //hX

	//遍历（类似遍历数组）
	for (int i = 0; i < vec1.size(); i++) 
		cout << vec1[i] << " "; // 1 10 3
	cout << endl;

	return 0;
}
