#include <iostream>
#include <string>
using namespace std;
class Student {//学生类
public:
	void eat(const string& food) {//吃
		cout << "我吃" << food << endl;
	}
	void sleep(const string& addr) {//睡
		cout << "我睡在" << addr << endl;
	}
	void study(const string& course) {//学习
		cout << "我学" << course << endl;
	}
private:
	string name; //姓名
	int age;	 //年龄
};
class Teacher {//教师类
public:
	void eat(const string& food) {//吃
		cout << "我吃" << food << endl;
	}
	void sleep(const string& addr) {//睡
		cout << "我睡在" << addr << endl;
	}
	void teach(const string& course) {//教学
		cout << "我教" << course << endl;
	}
private:
	string name; //姓名
	int age;	 //年龄
};
int main() {
	Student stu;
	stu.eat("快餐");
	stu.sleep("宿舍");
	stu.study("c++");
	Teacher tea;
	tea.eat("小炒");
	tea.sleep("家里");
	tea.teach("C++");
	return 0;
}