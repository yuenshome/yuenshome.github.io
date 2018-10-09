#include <iostream>
#include <vector>
#include <string>
using namespace std;
struct Stu {	//单个学生结构体
	int id;
	string name;
};
int main() {
	vector<Stu> ss;
	Stu stu1;
	stu1.id = 1; stu1.name = "张三";
	ss.push_back(stu1);
	stu1.id = 5; stu1.name = "mike";
	ss.push_back(stu1);
	for (int i = 0; i < ss.size(); i++) {
		cout << "(" << ss[i].id << ",";
		cout << ss[i].name << ")\t";
	}
	cout << endl;
	return 0;
}